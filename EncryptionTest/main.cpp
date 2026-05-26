#include<iostream>

#include"Crypto.hpp"
#include"Log.hpp"

const std::string path{"D:\\Crypto\\SampleFiles"};
const std::string cryptoKey{"2B7E151628AED2A6ABF7158809CF4F3C"};
const std::string blockCipher{"AES-128/CBC/PKCS7"};

void encryptionApplicationTest();
void loggingApplicationTest();

int main()
{
	//loggingApplicationTest();
	encryptionApplicationTest();

	return 0;
}

void encryptionApplicationTest()
{
	DirectoryEncryptor fcrypt(cryptoKey, blockCipher);
	int command{-1};

	while (command)
	{
		std::cout << "\nEnter 1 for decryption 2 for encryption, 0 to quit\n";
		std::cout << "Command: ";
		std::cin >> command;
		std::cout << '\n';

		switch (command)
		{
		case 1:
			fcrypt.decrypt(std::filesystem::path(path));
			break;
		case 2:
			fcrypt.encrypt(std::filesystem::path(path));
			break;
		default:
			break;
		}
	}
}

void loggingApplicationTest()
{
	Log logTest{};
	int userInput{-1};

	while (userInput)
	{
		std::cout << "0 - QUIT 1 - TEST LOGGING ENGINE 2 - CHANGE LOG DESTINATION\n";
		std::cin >> userInput;

		switch (userInput)
		{
		case 1:
			logTest.logMessage("Hello World from main application - logging test");
			break;
		case 2:
			{
				std::time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				std::string dummyIdentifier = std::to_string(systemTime);

				logTest.moveLog(dummyIdentifier);
			}
			break;
		default:
			break;
		}
	}
}
