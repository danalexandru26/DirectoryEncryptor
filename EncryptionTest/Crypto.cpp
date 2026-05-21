#include"Crypto.hpp"

DirectoryEncryptor::DirectoryEncryptor() {};

DirectoryEncryptor::DirectoryEncryptor(const std::string& key, const std::string& path) : path { path } {
	cryptoKey = Botan::hex_decode_locked(key);
	initializeBotan();
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
	enc = Botan::Cipher_Mode::create_or_throw("AES-128/CBC/PKCS7", Botan::Cipher_Dir::Encryption);
	enc->set_key(cryptoKey);
	dec = Botan::Cipher_Mode::create_or_throw("AES-128/CBC/PKCS7", Botan::Cipher_Dir::Decryption);
	dec->set_key(cryptoKey);
}

int DirectoryEncryptor::EncryptDirectory() {
	namespace fs = std::filesystem;

	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::cout << std::format("File read: {}\n", entry.path().filename().string());
			encryptFile(entry.path());
		}
	}
}

int DirectoryEncryptor::DecryptDirectory() {

}

int DirectoryEncryptor::encryptFile(const std::filesystem::path& fPath) {
	std::fstream file(fPath);

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
	const auto iv = Botan::hex_decode_locked("434B5966EC35D5C0F7CD4ABD63FD70B2");
	std::cout << "IV: " << Botan::hex_encode(iv) << '\n';

	std::vector<char> buffer(_CHUNK_);

	enc->start(iv);
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
