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

bool ReadRaw(const std::filesystem::path& path)
{
	std::fstream wStream{};

	wStream.open(path, std::ios::out | std::ios::binary);
	if (!wStream.is_open())
	{
		std::cerr << "Could not perform test configuration by writing to file: " << path << '\n';
		return false;
	}

	char wData{'@'};
	wStream.write(&wData, 1);
	wStream.close();

	{
		FileReader reader(path);

		if (reader.status())
		{
			char rData{};
			reader.readRaw(&rData, 1);
			compare(wData, rData);
		}
	}
	Test_cleanup(path);
	return true;
}

bool ReadArray(const std::filesystem::path& path)
{
	std::fstream wStream{};

	wStream.open(path, std::ios::out | std::ios::binary);
	if (!wStream.is_open())
	{
		std::cerr << "Could not perform test configuration by writing to file: " << path << '\n';
		return false;
	}

	std::vector<char> wData{'t', 'e', 's', 't'};
	wStream.write(wData.data(), wData.size());
	wStream.close();

	{
		FileReader reader(path);

		if (reader.status())
		{
			std::vector<char> rData{};
			rData.resize(wData.size());

			rData = reader.readRawArray(rData.size());
			compare(wData, rData);
		}
	}
	Test_cleanup(path);
	return true;
}

bool ReadAll(const std::filesystem::path& path)
{
	FileReader reader(path);

	try
	{
		std::vector<char> buffer = reader.readAll();
	}
	catch (std::exception& e)
	{
		std::cerr << std::format("Error: {} || Location: {}", e.what(),
		                         std::source_location::current().function_name());
		return false;
	}
	return true;
}

bool ReadChunk(const std::filesystem::path& path)
{
	constexpr uint16_t _chunk = 4096;

	std::vector<char> buffer(_chunk);
	FileReader reader(path);

	reader.readChunk(buffer.data(), _chunk);
	std::cout << buffer.data() << '\n';

	return true;
}

bool ReadAllChunks(const std::filesystem::path& path)
{
	constexpr uint16_t _chunk = 2;
	std::vector<char> buffer(_chunk);

	std::ifstream file(path, std::ios::in | std::ios::binary);

	while (file.read(buffer.data(), _chunk) || file.gcount() > 0)
	{
		file.peek();
		if (file.eof())
		{
			std::cerr << "\nEnd of file reached " << buffer.data() << '\n';
		}
		else
		{
			std::cerr << "Not at EOF: " << buffer.data() << '\n';
		}
	}
	return true;
}

void CloudflareUpload(const std::string& path, const std::string& cloudFilename)
{
	
}

void Test_cleanup(const std::filesystem::path& path)
{
	std::filesystem::remove(path);
}
