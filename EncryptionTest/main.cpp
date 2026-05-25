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
	int command{ -1 };

	while (command) {
		std::cout << "\nEnter 1 for decryption 2 for encryption, 0 to quit\n";
		std::cout << "Command: ";
		std::cin >> command;
		std::cout << '\n';

		switch (command) {
		case 1:
			fcrypt.DecryptDirectory(std::filesystem::path(path));
			fcrypt.decryptionErrors();
			break;
		case 2:
			fcrypt.EncryptDirectory(std::filesystem::path(path));
			fcrypt.encryptionErrors();
			break;
		default:
			break;
		}
	}
	return 0;
}