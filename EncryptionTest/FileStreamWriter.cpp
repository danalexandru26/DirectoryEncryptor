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

void swap(FileStreamWriter& lhs, FileStreamWriter& rhs) noexcept
{
	std::swap(lhs.mPath, rhs.mPath);
	lhs.mFile.swap(rhs.mFile);
}
