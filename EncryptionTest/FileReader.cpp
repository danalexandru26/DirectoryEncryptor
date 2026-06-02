#include "FileReader.hpp"

FileReader::FileReader(const std::filesystem::path& path) : mPath{path}
{
	mFile.open(path);
}

FileReader::FileReader(FileReader&& other) noexcept : FileReader()
{
	swap(*this, other);
}

FileReader::~FileReader()
{
	if (mFile.is_open())
	{
		mFile.close();
	}
}

FileReader& FileReader::operator=(FileReader other)
{
	swap(*this, other);
	return *this;
}

void FileReader::swap(FileReader& lhs, FileReader& rhs)
{
	std::swap(lhs.mPath, rhs.mPath);
	mFile.swap(rhs.mFile);
}

int FileReader::status()
{
	return mFile.is_open();
}

std::ios_base::iostate FileReader::state()
{
	return mFile.eof();
}

std::vector<char> FileReader::readAll(std::streamsize limit)
{
	mFile.seekg(0, std::ios::end);
	std::streamsize size = mFile.tellg();
	mFile.seekg(0, std::ios::beg);

	if (size > limit)
	{
		throw std::runtime_error("File size exceeds the limit");
	}
	std::vector<char> buffer(size);

	mFile.read(buffer.data(), size);
	return buffer;
}

std::streamsize FileReader::readChunk(char* buffer, std::size_t chunk)
{
	mFile.read(buffer, chunk);
	return mFile.gcount();
}

void FileReader::readRaw(char* destination, std::size_t count)
{
	mFile.read(destination, count);
}

std::vector<char> FileReader::readRawArray(std::size_t count)
{
	std::vector<char> buffer(count);

	mFile.read(buffer.data(), count);

	return buffer;
}

std::string FileReader::readString(std::size_t count)
{
	std::string string;
	string.resize(count);

	mFile.read(string.data(), count);
	return string;
}

void FileReader::moveResources(FileReader& other)
{
	if (this != &other)
	{
		mPath = std::move(other.mPath);
		mFile = std::move(other.mFile);
	}
}
