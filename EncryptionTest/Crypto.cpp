#include"Crypto.hpp"

DirectoryEncryptor::DirectoryEncryptor(const std::string& key, const std::string& cipher)
{
	this->cipher = cipher;
	cryptoKey = Botan::hex_decode_locked(key);
};

DirectoryEncryptor& DirectoryEncryptor::operator=(DirectoryEncryptor&& other) noexcept
{
	if (this != & other)
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

void DirectoryEncryptor::initializeLog(const std::filesystem::path& path)
{
	LOG_ = Log(
		path);
}

bool DirectoryEncryptor::decryptFile(const std::filesystem::path& fPath)
{
	if (verifyExclusion(fPath))
	{
		std::cerr << "File extension is in the exclusion list. Path: " << fPath << '\n';
		return false;
	}

	std::fstream file(fPath, std::ios::binary | std::ios::in);
	if (!file)
	{
		std::cerr << "Invalid Path: " << fPath << '\n';
		return false;
	}

	std::filesystem::path dummyPath{fPath};
	dummyPath.replace_extension(_TMPEXT_);

	std::ofstream dummyFile(dummyPath, std::ios::out | std::ios::binary);
	if (!file)
	{
		std::cerr << "Temporary file cannot be created. Decryption failed for: " << fPath << '\n';
		return false;
	}
	std::cerr << "Currently Decrypting: " << fPath << '\n';

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
			if (file.eof())
			{
				dec->finish(pt);
			}
			else dec->update(pt);
			dummyFile.write(reinterpret_cast<const char*>(pt.data()), pt.size());
		}
		catch (std::exception& e)
		{
			//TODO
		}
	}

	file.close();
	dummyFile.close();

	std::filesystem::remove(fPath);
	std::filesystem::rename(dummyPath, fPath);

	return true;
}

bool DirectoryEncryptor::encryptFile(const std::filesystem::path& fPath)
{
	if (verifyExclusion(fPath))
	{
		std::cerr << "File extension is in the exclusion list. Path: " << fPath << '\n';
		return false;
	}

	std::fstream file(fPath, std::ios::in | std::ios::binary);
	if (!file)
	{
		std::cerr << "Invalid Path: " << fPath << '\n';
		return false;
	}
	std::filesystem::path dummyPath{fPath};
	dummyPath.replace_extension(_TMPEXT_);
	std::ofstream dummyFile(dummyPath, std::ios::out | std::ios::binary);

	if (!dummyFile)
	{
		std::cerr << "Temporary file cannot be created. Encryption failed for: " << fPath << '\n';
		return false;
	}
	std::cerr << "Currently Encrypting: " << fPath << '\n';

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
			//TODO
		}
	}

	file.close();
	dummyFile.close();

	std::filesystem::remove(fPath);
	std::filesystem::rename(dummyPath, fPath);
	METADATA_.record(fPath, {JSONKeys::Timestamp, JSONKeys::Filesize});
	return true;
}

bool DirectoryEncryptor::verifyExclusion(const std::filesystem::path& fPath)
{
	for (auto& e : exclusions)
	{
		if (fPath.extension() == e)
		{
			return true;
		}
	}
	return false;
}
