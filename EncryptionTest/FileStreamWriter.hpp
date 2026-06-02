#include<fstream>
#include<filesystem>
#include<vector>
#include<string>
#include<cstdint>

class FileStreamWriter
{
public:
	FileStreamWriter() = default;

private:
	std::filesystem::path mPath;
	std::ofstream mFile;
};
