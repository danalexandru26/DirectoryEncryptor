#include"Log.hpp"

Log::Log()
{
	manifest = std::filesystem::path(_DEFAULT_LOG_PATH_);
}

Log::~Log()
{	
	if (fileHandle) {
		for (auto& i : logCache)
		{
			fileHandle << i << '\n';
		}
		fileHandle.close();
	}
}

bool Log::initFileHandle()
{
	fileHandle.open(manifest, std::ios::out | std::ios::in | std::ios::app);

	if (!fileHandle.is_open())
	{
		return false;
	}
	return true;
}

bool Log::logMessage(const std::string& message)
{
	if (!fileHandle.is_open())
	{
		bool result = this->initFileHandle();
		std::cerr << "There is no file handle to the log descriptor. Attempting to open file\n";
		if (!result)
		{
			std::cerr << "Log file handle cannot be opened\n";
			return false;
		}
		std::cerr << "File handle successfully opened\n";
	}
	fileHandle << "Severity: " << Severity::INFO << ' ' << message << '\n';

	return true;
}

bool Log::moveLog(const std::filesystem::path& path)
{
	if (!fileHandle.is_open())
	{
		std::cerr << "Log file handle is inaccessible, transfer of log data cannot be performed\n";
		return false;
	}

	std::fstream dummyHandle(path, std::ios::out | std::ios::in | std::ios::app);
	if (!dummyHandle.is_open())
	{
		std::cerr << "New log destination file handle cannot be opened or created\n";
		return false;
	}

	fileHandle.seekg(0, std::ios::beg);
	std::vector<char> buffer(_LOG_CHUNK_);

	while (fileHandle.read(buffer.data(), _LOG_CHUNK_) || fileHandle.gcount() > 0)
	{
		std::streamsize size = fileHandle.gcount();
		dummyHandle.write(buffer.data(), size);
	}
	

	fileHandle = std::move(dummyHandle);
	manifest = path;

	return true;
}
