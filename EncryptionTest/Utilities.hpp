#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<filesystem>

#include"HeaderContainer.hpp"
#include"FileStreamWriter.hpp"
#include"Crypto.hpp"
#include"JMetadata.hpp"

inline static const std::string path{"D:\\Crypto\\SampleFiles"};
inline static const std::string cryptoKey{"2B7E151628AED2A6ABF7158809CF4F3C"};
inline static const std::string blockCipher{"AES-128/CBC/PKCS7"};

void encryptionApplicationTest();
void loggingApplicationTest();
void fileStreamApplicationTest();

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
