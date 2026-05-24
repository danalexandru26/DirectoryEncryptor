#include"Crypto.hpp"

DirectoryEncryptor::DirectoryEncryptor() {};

DirectoryEncryptor::DirectoryEncryptor(const std::string& key, const std::string& cipher) {
	this->cipher = cipher;
	cryptoKey = Botan::hex_decode_locked(key);
	initializeBotan();
};

void DirectoryEncryptor::initializeBotan() {
	enc = Botan::Cipher_Mode::create_or_throw(cipher, Botan::Cipher_Dir::Encryption);
	enc->set_key(cryptoKey);
	dec = Botan::Cipher_Mode::create_or_throw(cipher, Botan::Cipher_Dir::Decryption);
	dec->set_key(cryptoKey);
}

int DirectoryEncryptor::EncryptDirectory(const std::filesystem::path& path) {
	namespace fs = std::filesystem;

	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			encryptFile(entry.path());
		}
		else if (entry.is_directory()) {
			this->EncryptDirectory(entry.path());
		}
	}
}

int DirectoryEncryptor::DecryptDirectory(const std::filesystem::path& path) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			this->decryptFile(entry.path());
		}
		else {
			this->DecryptDirectory(entry.path());
		}
	}
}

void DirectoryEncryptor::excludeExtension(const std::string& extension) {
	std::filesystem::path ext(extension);
	exclusions.insert(ext);
}

void DirectoryEncryptor::excludeExtension(const std::vector<std::string>& extensions) {
	for (auto& e : extensions) {
		std::filesystem::path ext(e);
		exclusions.insert(ext);
	}
}

bool DirectoryEncryptor::decryptFile(const std::filesystem::path& fPath) {
	if (verifyExclusion(fPath)) {
		std::cerr << "File extension is in the exclusion list. Path: " << fPath << '\n';
		return false;
	}

	std::fstream file(fPath, std::ios::binary | std::ios::in);
	if (!file) {
		std::cerr << "Invalid Path: " << fPath << '\n';
		return false;
	}
	
	std::filesystem::path dummyPath{ fPath };
	dummyPath.replace_extension(_TMPEXT_);

	std::ofstream dummyFile(dummyPath, std::ios::out | std::ios::binary);
	if (!file) {
		std::cerr << "Temporary file cannot be created. Decryption failed for: " << fPath << '\n';
		return false;
	}
	std::cerr << "Currently Decrypting: " << fPath << '\n';

	std::vector<char> buffer(_CHUNK_);
	std::vector<uint8_t> iv(_IVLEN_);

	file.read(reinterpret_cast<char*>(iv.data()), iv.size());
	dec->start(iv);

	while (!file.eof()) {
		file.read(buffer.data(), _CHUNK_);
		std::streamsize size = file.gcount();

		Botan::secure_vector<uint8_t> pt(buffer.begin(), buffer.begin() + size);

		if (file.eof()) {
			dec->finish(pt);
		}
		else dec->update(pt);

		dummyFile.write(reinterpret_cast<const char*>(pt.data()), pt.size());
	}

	file.close();
	dummyFile.close();

	std::filesystem::remove(fPath);
	std::filesystem::rename(dummyPath, fPath);

	return true;
}

bool DirectoryEncryptor::encryptFile(const std::filesystem::path& fPath) {
	if (verifyExclusion(fPath)) {
		std::cerr << "File extension is in the exclusion list. Path: " << fPath << '\n';
		return false;
	}

	std::fstream file(fPath, std::ios::in | std::ios::binary);
	if (!file) {
		std::cerr << "Invalid Path: " << fPath << '\n';
		return false;
	}

	std::filesystem::path dummyPath{ fPath };
	dummyPath.replace_extension(_TMPEXT_);

	std::ofstream dummyFile(dummyPath, std::ios::out | std::ios::binary);
	if (!dummyFile) {
		std::cerr << "Temporary file cannot be created. Encryption failed for: " << fPath << '\n';
		return false;
	}
	std::cerr << "Currently Encrypting: " << fPath << '\n';

	Botan::AutoSeeded_RNG rng;
	const auto iv = rng.random_vec<std::vector<uint8_t>>(enc->default_nonce_length());
	std::vector<char> buffer(_CHUNK_);

	enc->start(iv);
	dummyFile.write(reinterpret_cast<const char*>(iv.data()), iv.size());

	while (!file.eof()) {
		file.read(buffer.data(), _CHUNK_);
		std::streamsize size = file.gcount();
		
		Botan::secure_vector<uint8_t> pt(buffer.begin(), buffer.begin() + size);
		
		if (file.eof()) {
			enc->finish(pt);
		}
		else enc->update(pt);

		dummyFile.write(reinterpret_cast<const char*>(pt.data()), pt.size());
	}
	file.close();
	dummyFile.close();

	std::filesystem::remove(fPath);
	std::filesystem::rename(dummyPath, fPath);

	return true;
}

bool DirectoryEncryptor::verifyExclusion(const std::filesystem::path& fPath) {
	for (auto& e : exclusions) {
		if (fPath.extension() == e) {
			return true;
		}
	}
	return false;
}