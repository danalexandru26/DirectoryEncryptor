#include<botan/auto_rng.h>
#include<botan/cipher_mode.h>
#include<botan/block_cipher.h>
#include<botan/hex.h>
#include<botan/rng.h>

#include<iostream>

#include"Crypto.hpp"

const std::string path{ "D:\\Crypto\\SampleFiles" };
const std::string cryptoKey{ "2B7E151628AED2A6ABF7158809CF4F3C" };
const std::string blockCipher{ "AES-128/CBC/PKCS7" };

int main() {
	DirectoryEncryptor fcrypt(cryptoKey, blockCipher);

	fcrypt.excludeExtension(".tmp");
	fcrypt.EncryptDirectory(std::filesystem::path(path));

	return 0;
}