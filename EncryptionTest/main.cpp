#include<iostream>

#include"Crypto.hpp"
#include"Utilities.hpp"
#include"SystemUtilities.hpp"

int main()
{
	const std::string envEncryption("AES128_ENCRYPTION_KEY");
	const std::string blockCipher{"AES-128/CBC/PKCS7"};

	const char* encryptionKey = readEnvironmentVariable(envEncryption);

	if (encryptionKey == nullptr)
	{
		std::cerr << "Encryption key environment variable is invalid\n";
		return 1;
	}
	encryptionApplicationTest(encryptionKey, blockCipher);

	delete[] encryptionKey;
	return 0;
}
