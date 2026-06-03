#include"FileStreamWriter.hpp"

FileStreamWriter::FileStreamWriter(const std::filesystem::path& path) : mPath{path}
{
	mFile.open(mPath);
}

FileStreamWriter::FileStreamWriter(FileStreamWriter&& other) noexcept : FileStreamWriter()
{
	swap(*this, other);
}

FileStreamWriter::~FileStreamWriter()
{
	if (mFile.is_open())
	{
		mFile.close();
	}
}

FileStreamWriter& FileStreamWriter::operator=(FileStreamWriter other)
{
	swap(*this, other);
	return *this;
}

void FileStreamWriter::writeData(const char* data, std::size_t size)
{
	mFile.write(data, size);
}

void FileStreamWriter::writeString(const std::string& string, std::size_t size)
{
	mFile.write(string.data(), size);
}

void swap(FileStreamWriter& lhs, FileStreamWriter& rhs) noexcept
{
	std::swap(lhs.mPath, rhs.mPath);
	lhs.mFile.swap(rhs.mFile);
}
