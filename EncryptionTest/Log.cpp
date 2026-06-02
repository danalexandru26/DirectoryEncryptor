#include"Log.hpp"

#include <source_location>

Log::Log()
{
	manifest = std::filesystem::path(_DEFAULT_LOG_PATH_);
	fileHandle.open(manifest, std::ios::app | std::ios::out | std::ios::in);
}

Log::Log(std::filesystem::path path)
{
	manifest = std::move(path);
	fileHandle.open(manifest, std::ios::in | std::ios::out | std::ios::app);
}

Log::~Log()
{
	if (fileHandle.is_open())
	{
		cacheTransfer();
		fileHandle.close();
	}
}

Log& Log::operator=(Log&& other) noexcept
{
	if (this != &other)
	{
		manifest = std::move(other.manifest);
		fileHandle = std::move(other.fileHandle);
		logCache = std::move(other.logCache);
		config = other.config;
	}

	return *this;
}

bool Log::logMessage(const std::string& scope, const std::string& message, uint8_t level,
                     const std::source_location& location)
{
	std::chrono::system_clock::time_point systemClock = std::chrono::system_clock::now();
	std::string logMessage = std::format("[{}] [{}] Scope[{}] Severity[{}]: {}\n", systemClock,
	                                     location.function_name(), scope, level,
	                                     message);

	if (level >= 2)
	{
		return directTransfer(logMessage);
	}

	logCache.push_back(logMessage);
	if (logCache.size() >= _LOG_ENTRY_MAX_)
	{
		return cacheTransfer();
	}

	return true;
}

bool Log::cacheTransfer()
{
	for (auto& log : logCache)
	{
		bool op = fileWrite(log);
		if (!op)return false;
	}
	fileHandle.flush();
	logCache.clear();

	return true;
}

bool Log::directTransfer(const std::string& message)
{
	bool op = fileWrite(message);
	if (op) fileHandle.flush();

	return op;
}

bool Log::fileWrite(const std::string& message)
{
	if (!fileHandle.is_open())
	{
		return false;
	}
	fileHandle.write(message.data(), message.size());

	return true;
}

bool Log::moveLog(const std::filesystem::path& path)
{
	if (!fileHandle.is_open())
	{
		std::cerr << "Log file handler cannot be instantiated\n";
		return false;
	}

	std::fstream dummyHandle(path, std::ios::app | std::ios::out | std::ios::in);
	if (!dummyHandle.is_open())
	{
		std::cerr << "New log destination file handle cannot instantiated\n";
		return false;
	}

	fileHandle.seekg(0, std::ios::beg);
	std::vector<char> buffer(_LOG_CHUNK_);

	while (fileHandle.read(buffer.data(), _LOG_CHUNK_) || fileHandle.gcount() > 0)
	{
		std::streamsize size = fileHandle.gcount();
		dummyHandle.write(buffer.data(), size);
	}
	manifest = path;
	fileHandle.close();
	fileHandle = std::move(dummyHandle);

	return true;
}

bool Log::isActive()
{
	return fileHandle.is_open();
}
