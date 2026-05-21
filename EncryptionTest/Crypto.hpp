#pragma once

#include<botan/auto_rng.h>
#include<botan/cipher_mode.h>
#include<botan/block_cipher.h>
#include<botan/hex.h>
#include<botan/rng.h>

#include<iostream>
#include<fstream>
#include<string>
#include<format>
#include<filesystem>

constexpr uint16_t _CHUNK_ = 1024;

class DirectoryEncryptor {
public:
	DirectoryEncryptor();
	DirectoryEncryptor(const std::string& key, const std::string& path);
	DirectoryEncryptor(const DirectoryEncryptor& other) = delete;
	DirectoryEncryptor& operator=(const DirectoryEncryptor& other) = delete;

	int EncryptDirectory();
	int DecryptDirectory();

private:
	void initializeBotan();
	int verifyPath();

	int encryptFile(const std::string& fPath);

private:
	Botan::secure_vector<uint8_t> cryptoKey{};
	std::unique_ptr<Botan::Cipher_Mode> enc{};
	std::unique_ptr<Botan::Cipher_Mode> dec{};
	std::string path{};
};