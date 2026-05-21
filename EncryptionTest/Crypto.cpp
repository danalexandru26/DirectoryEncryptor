#include"Crypto.hpp"

DirectoryEncryptor::DirectoryEncryptor() {};

DirectoryEncryptor::DirectoryEncryptor(const std::string& key, const std::string& path) : path { path } {
	cryptoKey = Botan::hex_decode_locked(key);
};

int DirectoryEncryptor::verifyPath() {
	if (path.length()) {
		namespace fs = std::filesystem;

		fs::path p(path);

		if (!fs::exists(p)) {
			return -2;
		}
	}
	return -1;
}

void DirectoryEncryptor::initializeBotan() {
	enc->clear();
	dec->clear();

	enc = Botan::Cipher_Mode::create_or_throw("AES-128/CBC/PKCS7", Botan::Cipher_Dir::Encryption);
	enc->set_key(cryptoKey);

	Botan::AutoSeeded_RNG rng;
	const auto iv = rng.random_vec<std::vector<uint8_t>>(enc->default_nonce_length());

	dec = Botan::Cipher_Mode::create_or_throw("AES-128/CBC/PKCS7", Botan::Cipher_Dir::Decryption);
	dec->set_key(cryptoKey);
}

int DirectoryEncryptor::EncryptDirectory() {
	namespace fs = std::filesystem;

	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::cout << std::format("File read: {}\n", entry.path().filename().string());
		}
	}
}

int DirectoryEncryptor::DecryptDirectory() {

}

int DirectoryEncryptor::encryptFile(const std::string& fPath) {
	std::fstream file(path);

	if (!file) {
		std::cout << std::format("The file path is invalid. Path: {}\n", fPath);
	}

	std::string buffer{};
	buffer.resize(_CHUNK_);

	while (file) {
		file.read(&buffer[0], _CHUNK_);

		std::streamsize bytesRead = file.gcount();

		if (bytesRead > 0) {

		}

	}
}
