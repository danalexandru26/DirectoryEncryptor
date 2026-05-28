#include"Utilities.hpp"

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
			logTest.logMessage("Global", "Hello World from main application - logging test", 0);
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

void fileStreamApplicationTest()
{
	std::filesystem::path path("fileStreamWriterTest.txt");
	FileStreamWriter fileStream(path);

	int userInput{-1};

	while (userInput)
	{
		std::cout << "0 - QUIT 1 - WRITE BYTE ARRAY 2 - WRITE STRING\n";
		std::cin >> userInput;

		switch (userInput)
		{
		case 0:
			std::cerr << "Exiting application\n";
			break;
		case 1:
			{
				HeaderContainer container;
				container.version = 1;
				container.IV = fillVector<uint8_t>(15, 100);

				fileStream.writeObject(container);
			}
			break;
		case 2:
			{
				std::string string{};

				std::cout << "Input string to serialize: ";
				std::cin >> string;

				fileStream.writeString(string);
			}
			break;
		}
	}
}
