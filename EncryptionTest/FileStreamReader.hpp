#pragma once
#include<cstdint>
#include<fstream>
#include<filesystem>
#include<string>

class FileStreamReader
{
public:
	FileStreamReader() = default;
	FileStreamReader(const std::filesystem::path& path);
	FileStreamReader(const FileStreamReader& other) = delete;
	FileStreamReader(FileStreamReader&& other) noexcept;
	~FileStreamReader();

	FileStreamReader& operator=(FileStreamReader other);

	void readRaw(char* destination, std::size_t count);
	std::streamsize readChunk(char* buffer, std::size_t chunk);
	std::vector<char> readAll(std::streamsize limit = 1024 * 1024 * 500);
	std::vector<char> readRawArray(std::size_t count);
	std::string readString(std::size_t count);

	void swap(FileStreamReader& lhs, FileStreamReader& rhs);
	int state();
	std::streamsize gcount();

private:
	std::filesystem::path mPath;
	std::fstream mFile;
};
