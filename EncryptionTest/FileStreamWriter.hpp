#include<fstream>
#include<filesystem>
#include<vector>
#include<string>
#include<cstdint>

class FileStreamWriter
{
public:
	FileStreamWriter() = default;
	FileStreamWriter(const std::filesystem::path& path);
	FileStreamWriter(const FileStreamWriter& other) = delete;
	FileStreamWriter(FileStreamWriter&& other) noexcept;
	~FileStreamWriter();

	FileStreamWriter& operator=(FileStreamWriter other);

	friend void swap(FileStreamWriter& lhs, FileStreamWriter& rhs) noexcept;
private:
	std::filesystem::path mPath;
	std::ofstream mFile;
};
