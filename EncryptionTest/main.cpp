#include<iostream>

#include"Crypto.hpp"
#include"Utilities.hpp"
#include"SystemUtilities.hpp"

int main()
{
	constexpr std::string envEncryption("AES128_ENCRYPTION_KEY");
	
	const char* encryptionKey = readEnvironmentVariable(envEncryption);

	if (encryptionKey != nullptr)
	{
		std::cerr << encryptionKey << '\n';
	}
	else
	{
		std::cerr << "Encryption key environment variable is invalid\n";
		return 1;
	}




	delete[] encryptionKey;
	return 0;
}
