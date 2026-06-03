#include"Crypto.hpp"

DirectoryEncryptor::DirectoryEncryptor(const std::string& key, const std::string& cipher)
{
	this->cipher = cipher;
	cryptoKey = Botan::hex_decode_locked(key);
};

DirectoryEncryptor& DirectoryEncryptor::operator=(DirectoryEncryptor&& other) noexcept
{
	if (this != &other)
	{
		cryptoKey = std::move(other.cryptoKey);
		enc = std::move(other.enc);
		dec = std::move(other.dec);
		exclusions = std::move(other.exclusions);
		cipher = std::move(other.cipher);

		LOG_ = std::move(other.LOG_);
		METADATA_ = std::move(other.METADATA_);
	}
	return *this;
}

void DirectoryEncryptor::encrypt(const std::filesystem::path& path)
{
	this->encryptDirectory(path);
	METADATA_.saveJM();
}

void DirectoryEncryptor::decrypt(const std::filesystem::path& path)
{
	this->decryptDirectory(path);
}

void DirectoryEncryptor::encryptDirectory(const std::filesystem::path& path)
{
	enc = Botan::Cipher_Mode::create_or_throw(cipher, Botan::Cipher_Dir::Encryption);
	enc->set_key(cryptoKey);

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			encryptFile(entry.path());
		}
		else if (entry.is_directory())
		{
			this->encryptDirectory(entry.path());
		}
	}
}

void DirectoryEncryptor::decryptDirectory(const std::filesystem::path& path)
{
	dec = Botan::Cipher_Mode::create_or_throw(cipher, Botan::Cipher_Dir::Decryption);
	dec->set_key(cryptoKey);

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			this->decryptFile(entry.path());
		}
		else
		{
			this->decryptDirectory(entry.path());
		}
	}
}

bool DirectoryEncryptor::decryptFile(const std::filesystem::path& path)
{
	if (verifyExclusion(path))
	{
		LOG_.logMessage("Decryption-Exclusion List", path.string(), 0);
		return false;
	}

	std::fstream file(path, std::ios::binary | std::ios::in);
	if (!file)
	{
		LOG_.logMessage("Decryption-Invalid File", path.string(), 1);
		return false;
	}

	std::filesystem::path dummyPath{path};
	dummyPath.replace_extension(_ENCODED);

	std::ofstream dummyFile(dummyPath, std::ios::out | std::ios::binary);
	if (!file)
	{
		LOG_.logMessage("Decryption", path.string(), 2);
		return false;
	}
	LOG_.logMessage("Decryption", path.string(), 0);
	LOG_.logMessage(std::cout, "Decryption", path.string(), 0);

	std::vector<char> buffer(_CHUNK_);
	std::vector<uint8_t> iv(_IVLEN_);

	file.read(reinterpret_cast<char*>(iv.data()), iv.size());
	dec->start(iv);

	while (file.read(buffer.data(), _CHUNK_) || file.gcount() > 0)
	{
		std::streamsize size = file.gcount();
		Botan::secure_vector<uint8_t> pt(buffer.begin(), buffer.begin() + size);
		try
		{
			file.peek();
			if (file.eof())
			{
				dec->finish(pt);
			}
			else dec->update(pt);
			dummyFile.write(reinterpret_cast<const char*>(pt.data()), pt.size());
		}
		catch (std::exception& e)
		{
			LOG_.logMessage("Decryption", path.string(), 3);
		}
	}

	file.close();
	dummyFile.close();

	std::filesystem::remove(path);
	std::filesystem::rename(dummyPath, path);

	return true;
}

bool DirectoryEncryptor::encryptFile(const std::filesystem::path& path)
{
	if (verifyExclusion(path))
	{
		std::cerr << "File extension is in the exclusion list. Path: " << path << '\n';
		return false;
	}

	SHA256(path, "SHA-256-DEC");
	std::fstream file(path, std::ios::in | std::ios::binary);
	if (!file)
	{
		std::cerr << "Invalid Path: " << path << '\n';
		return false;
	}
	std::filesystem::path dummyPath{path};
	dummyPath.replace_extension(_ENCODED);
	std::ofstream dummyFile(dummyPath, std::ios::out | std::ios::binary);

	if (!dummyFile)
	{
		std::cerr << "Temporary file cannot be created. Encryption failed for: " << path << '\n';
		return false;
	}
	LOG_.logMessage("Decryption", path.string(), 0);
	LOG_.logMessage(std::cout, "Decryption", path.string(), 0);

	Botan::AutoSeeded_RNG rng;
	const auto iv = rng.random_vec<std::vector<uint8_t>>(enc->default_nonce_length());
	std::vector<char> buffer(_CHUNK_);

	enc->start(iv);
	dummyFile.write(reinterpret_cast<const char*>(iv.data()), iv.size());

	while (file.read(buffer.data(), _CHUNK_) || file.gcount() > 0)
	{
		std::streamsize size = file.gcount();
		Botan::secure_vector<uint8_t> pt(buffer.begin(), buffer.begin() + size);

		try
		{
			file.peek();
			if (file.eof())
			{
				enc->finish(pt);
			}
			else
			{
				enc->update(pt);
			}
			dummyFile.write(reinterpret_cast<const char*>(pt.data()), pt.size());
		}
		catch (std::exception& e)
		{
			LOG_.logMessage("Encryption", path.string(), 3);
		}
	}

	file.close();
	dummyFile.close();

	std::filesystem::remove(path);
	std::filesystem::rename(dummyPath, path);

	METADATA_.record(path, {JSONKeys::Timestamp, JSONKeys::Filesize});
	SHA256(path, "SHA-256-ENC");
	//uploadToCloudflareR2(fPath.generic_string(), fPath.generic_string());
	return true;
}

void DirectoryEncryptor::SHA256(const std::filesystem::path& path, const std::string& mode)
{
	const auto sha256 = Botan::HashFunction::create_or_throw("SHA-256");
	std::ifstream file(path);

	if (!file.is_open())
	{
		LOG_.logMessage("SHA-256", "Hash/Checksum could not be computed", 1);
		LOG_.logMessage(std::cout, "SHA - 256", "Hash / Checksum could not be computed", 1);
	}
	std::vector<uint8_t> buffer(_CHUNK_);

	while (file.read(reinterpret_cast<char*>(buffer.data()), _CHUNK_) || file.gcount() > 0)
	{
		sha256->update(std::span{buffer}.first(file.gcount()));
	}

	std::string hash = Botan::hex_encode(sha256->final());
	METADATA_.record(path, mode, hash);
	file.close();
}

void DirectoryEncryptor::excludeExtension(const std::string& extension)
{
	exclusions.insert(std::filesystem::path(extension));
}

void DirectoryEncryptor::excludeExtension(const std::vector<std::string>& extensions)
{
	for (auto& e : extensions)
	{
		exclusions.insert(std::filesystem::path(e));
	}
}

bool DirectoryEncryptor::verifyExclusion(const std::filesystem::path& path)
{
	for (auto& e : exclusions)
	{
		if (path.extension() == e)
		{
			return true;
		}
	}
	return false;
}

void DirectoryEncryptor::initializeLog(const std::filesystem::path& path)
{
	LOG_ = Log(path);
}

void DirectoryEncryptor::initializeMetadata(const std::filesystem::path& path)
{
	METADATA_ = JMetadata(path);
}
