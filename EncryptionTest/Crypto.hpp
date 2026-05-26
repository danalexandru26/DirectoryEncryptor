#pragma once

#include<botan/auto_rng.h>
#include<botan/cipher_mode.h>
#include<botan/block_cipher.h>
#include<botan/hex.h>
#include<botan/rng.h>

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<set>
#include<filesystem>

#include"JMetadata.hpp"

constexpr uint16_t _CHUNK_ = 1024;
constexpr uint16_t _IVLEN_ = 16;
inline const char* _TMPEXT_ = ".encr";

class DirectoryEncryptor {
public:
	DirectoryEncryptor();
	DirectoryEncryptor(const std::string& key, const std::string& cipher);
	DirectoryEncryptor(const DirectoryEncryptor& other) = delete;
	DirectoryEncryptor& operator=(const DirectoryEncryptor& other) = delete;

	void encrypt(const std::filesystem::path& path);
	void decrypt(const std::filesystem::path& path);
	void excludeExtension(const std::string& extension);
	void excludeExtension(const std::vector<std::string>& extensions);

private:
	void encryptDirectory(const std::filesystem::path& path);
	void decryptDirectory(const std::filesystem::path& path);
	bool encryptFile(const std::filesystem::path& fPath);
	bool decryptFile(const std::filesystem::path& fPath);
	bool verifyExclusion(const std::filesystem::path& fPath);

private:
	Botan::secure_vector<uint8_t> cryptoKey{};
	std::unique_ptr<Botan::Cipher_Mode> enc{};
	std::unique_ptr<Botan::Cipher_Mode> dec{};
	std::set<std::filesystem::path> exclusions{};
	std::string cipher{};

	std::vector<std::string> errorInfo{};

	JMetadata manifest{};
};