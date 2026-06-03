#include "FileStreamReader.hpp"

FileStreamReader::FileStreamReader(const std::filesystem::path& path) : mPath{path}
{
	mFile.open(path, std::ios::in | std::ios::binary);
}

FileStreamReader::FileStreamReader(FileStreamReader&& other) noexcept : FileStreamReader()
{
	swap(*this, other);
}

FileStreamReader::~FileStreamReader()
{
	if (mFile.is_open())
	{
		mFile.close();
	}
}

FileStreamReader& FileStreamReader::operator=(FileStreamReader other)
{
	swap(*this, other);
	return *this;
}

void FileStreamReader::swap(FileStreamReader& lhs, FileStreamReader& rhs)
{
	std::swap(lhs.mPath, rhs.mPath);
	lhs.mFile.swap(rhs.mFile);
}

std::vector<char> FileStreamReader::readAll(std::streamsize limit)
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

std::streamsize FileStreamReader::readChunk(char* buffer, std::size_t chunk)
{
	mFile.read(buffer, chunk);
	return mFile.gcount();
}

void FileStreamReader::readRaw(char* destination, std::size_t count)
{
	mFile.read(destination, count);
}

std::vector<char> FileStreamReader::readRawArray(std::size_t count)
{
	std::vector<char> buffer(count);

	mFile.read(buffer.data(), count);

	return buffer;
}

std::string FileStreamReader::readString(std::size_t count)
{
	std::string string;
	string.resize(count);

	mFile.read(string.data(), count);
	return string;
}

int FileStreamReader::state()
{
	return mFile.is_open();
}

std::streamsize FileStreamReader::gcount()
{
	return mFile.gcount();
}
