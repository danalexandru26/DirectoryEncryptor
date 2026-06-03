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
	FileStreamWriter& operator=(const FileStreamWriter& other) = delete;

	template <typename T>
	void writeRaw(const T& type)
	{
		if constexpr (std::is_trivial<T>())
		{
			writeData(reinterpret_cast<const char*>(&type), sizeof(T));
		}
	}

	template <typename T>
	void writeArray(const std::vector<T>& vector)
	{
		for (auto& v : vector)
		{
			if constexpr (std::is_trivial<T>())
			{
				writeData(v, sizeof(T));
			}
			else
			{
				// TODO Object serialization or architectural changes to allow writing of raw user defined types
			}
		}
	}

	void writeString(const std::string& string, std::size_t size);
	void writeData(const char* data, std::size_t size);

	friend void swap(FileStreamWriter& lhs, FileStreamWriter& rhs) noexcept;

private:
	std::filesystem::path mPath;
	std::ofstream mFile;
};
