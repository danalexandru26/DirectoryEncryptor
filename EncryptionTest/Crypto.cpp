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

int DirectoryEncryptor::DecryptDirectory() {

}

int DirectoryEncryptor::encryptFile(const std::filesystem::path& fPath) {
	std::fstream file(fPath, std::ios::in | std::ios::binary);

	if (!file) {
		std::cerr << "Invalid Path: " << fPath << '\n';
		return -1;
	}

	std::filesystem::path cpy{ fPath };
	std::filesystem::path ext(".tmp");
	cpy.replace_extension(ext);

	std::ofstream oFile;
	oFile.open(cpy, std::ios::binary);

	Botan::AutoSeeded_RNG rng;
	const auto iv = rng.random_vec<std::vector<uint8_t>>(enc->default_nonce_length());
	std::cout << "IV: " << Botan::hex_encode(iv) << '\n';

	std::vector<char> buffer(_CHUNK_);

	enc->start(iv);
	oFile.write(reinterpret_cast<const char*>(iv.data()), iv.size());

	while (!file.eof()) {
		file.read(buffer.data(), _CHUNK_);
		std::streamsize size = file.gcount();
		
		Botan::secure_vector<uint8_t> pt(buffer.begin(), buffer.begin() + size);
		
		if (file.eof()) {
			enc->finish(pt);
		}
		else enc->update(pt);

		oFile.write(reinterpret_cast<const char*>(pt.data()), pt.size());
	}

	file.close();
	oFile.close();
}
