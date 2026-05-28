#include"FileStreamWriter.hpp"

FileStreamWriter::FileStreamWriter(std::filesystem::path& path)
{
	mPath = std::move(path);
	file.open(mPath, std::ios::out | std::ios::binary);
}

FileStreamWriter::~FileStreamWriter()
{
	if (file.is_open())
	{
		file.close();
	}
}

FileStreamWriter::FileStreamWriter(FileStreamWriter&& other) noexcept
{
	if (this != &other)
	{
		moveData(other);
	}
}

FileStreamWriter& FileStreamWriter::operator=(FileStreamWriter&& other) noexcept
{
	if (this != &other)
	{
		moveData(other);
	}
	return *this;
}

bool FileStreamWriter::writeData(const char* data, std::size_t size)
{
	try {
		file.write(data, size);
	}
	catch (std::exception& e)
	{
		std::cerr << "Writing failure: " << e.what() << '\n';
		return false;
	}

	return true;
}

void FileStreamWriter::moveData(FileStreamWriter& other)
{
	mPath = std::move(other.mPath);
	file = std::move(other.file);
}
