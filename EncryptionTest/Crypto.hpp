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
	DirectoryEncryptor(const std::string& path);
	DirectoryEncryptor(const DirectoryEncryptor& other) = delete;
	DirectoryEncryptor& operator=(const DirectoryEncryptor& other) = delete;

	void setPath(const std::string& dirPath);
	int EncryptDirectory();
	int DecryptDirectory();

private:
	int verifyPath();

private:
	std::string path{};
};