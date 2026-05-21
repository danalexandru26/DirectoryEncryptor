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

constexpr uint16_t _CHUNK_ = 1024;

class DirectoryEncryptor {
public:
	DirectoryEncryptor();
	DirectoryEncryptor(const DirectoryEncryptor& other) = delete;
	DirectoryEncryptor& operator=(const DirectoryEncryptor& other) = delete;

private:

};