#pragma once
#include<cstdint>
#include<fstream>
#include<filesystem>
#include<string>

class FileReader
{
public:
	FileReader() = default;
	FileReader(const std::filesystem::path& path);
	FileReader(const FileReader& other) = delete;
	FileReader(FileReader&& other) noexcept;
	~FileReader();

	FileReader& operator=(FileReader other);

	void readRaw(char* destination, std::size_t count);
	std::streamsize readChunk(char* buffer, std::size_t chunk);
	std::vector<char> readAll(std::streamsize limit = 1024 * 1024 * 500);
	std::vector<char> readRawArray(std::size_t count);
	std::string readString(std::size_t count);

	void swap(FileReader& lhs, FileReader& rhs);
	int status();
	std::ios_base::iostate state();


private:
	void moveResources(FileReader& other);

	std::filesystem::path mPath;
	std::fstream mFile;
};
