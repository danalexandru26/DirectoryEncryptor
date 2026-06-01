#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<format>
#include<filesystem>
#include<concepts>

#include"HeaderContainer.hpp"
#include"Crypto.hpp"
#include"FileReader.hpp"

inline static const std::string path{"D:\\Crypto\\SampleFiles"};
inline static const std::string cryptoKey{"2B7E151628AED2A6ABF7158809CF4F3C"};
inline static const std::string blockCipher{"AES-128/CBC/PKCS7"};

template <typename T>
std::vector<T> fillVector(T value, std::size_t size)
{
	std::vector<T> buffer{};

	for (std::size_t i = 0; i < size; ++i)
	{
		buffer.push_back(value);
	}

	return buffer;
}

template <std::equality_comparable T>
bool compare(const T& left, const T& right)
{
	static_assert(std::equality_comparable<T>, "Type does not implement the equality test operator(==)");
	if (left == right)
	{
		std::cerr << "Left and Right operands are identical\n";
		return true;
	}
	std::cerr << "Left and Right operands differ\n";
	return false;
}

void encryptionApplicationTest();
void loggingApplicationTest();
void fileStreamApplicationTest();


bool ReadRaw(const std::filesystem::path& path);
bool ReadArray(const std::filesystem::path& path);
bool ReadAll(const std::filesystem::path& path);
bool ReadChunk(const std::filesystem::path& path);
bool ReadAllChunks(const std::filesystem::path& path);
void Test_cleanup(const std::filesystem::path& path);
