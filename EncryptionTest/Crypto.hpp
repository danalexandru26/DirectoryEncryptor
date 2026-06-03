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

#include"Log.hpp"
#include"JMetadata.hpp"
#include"CloudflareR2.hpp"
#include"FileStreamWriter.hpp"
#include"FileStreamReader.hpp"

constexpr uint32_t _CHUNK_ = 65536;
constexpr uint16_t _IVLEN_ = 16;
constexpr auto _TMPEXT_ = ".encr";

class DirectoryEncryptor
{
public:
	DirectoryEncryptor() = default;
	~DirectoryEncryptor() = default;
	DirectoryEncryptor(const std::string& key, const std::string& cipher);
	DirectoryEncryptor(const DirectoryEncryptor& other) = delete;

	DirectoryEncryptor& operator=(const DirectoryEncryptor& other) = delete;
	DirectoryEncryptor& operator=(DirectoryEncryptor&& other) noexcept;

	void encrypt(const std::filesystem::path& path);
	void decrypt(const std::filesystem::path& path);
	void excludeExtension(const std::string& extension);
	void excludeExtension(const std::vector<std::string>& extensions);
	void initializeLog(const std::filesystem::path& path);
	void initializeMetadata(const std::filesystem::path& path);

private:
	void encryptDirectory(const std::filesystem::path& path);
	void decryptDirectory(const std::filesystem::path& path);
	bool encryptFile(const std::filesystem::path& fPath);
	bool decryptFile(const std::filesystem::path& fPath);
	bool verifyExclusion(const std::filesystem::path& fPath);

	Botan::secure_vector<uint8_t> cryptoKey{};
	std::unique_ptr<Botan::Cipher_Mode> enc{};
	std::unique_ptr<Botan::Cipher_Mode> dec{};
	std::set<std::filesystem::path> exclusions{};
	std::string cipher{};

	Log LOG_{};
	JMetadata METADATA_{};
};
