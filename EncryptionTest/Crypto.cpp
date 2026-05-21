#include"Crypto.hpp"

DirectoryEncryptor::DirectoryEncryptor() {};

DirectoryEncryptor::DirectoryEncryptor(const std::string& path) : path{ path } {};

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

void DirectoryEncryptor::setPath(const std::string& dirPath) {
	path = dirPath;
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
