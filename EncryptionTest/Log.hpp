#pragma once
#include<iostream>
#include<filesystem>
#include<fstream>
#include<string>
#include<format>
#include<source_location>

constexpr uint16_t _LOG_CHUNK_ = 4096;
constexpr uint16_t _LOG_ENTRY_MAX_ = 2;
const std::string _DEFAULT_LOG_PATH_ = "execution_log.txt";

struct LogConfiguration
{
	bool verbosity{};
	bool toConsole{};
	bool toFile{};
	bool showTimestamp{};
	bool useCache{};

	std::size_t cacheSize{};
	uint16_t chunkSize{};
	uint8_t severityThreshold{};
};

class Log
{
public:
	Log();
	Log(std::filesystem::path path);
	virtual ~Log();

	Log(const Log& other) = delete;
	Log& operator=(const Log& other) = delete;
	Log& operator=(Log&& other) noexcept;

	bool logMessage(const std::string& scope, const std::string& message, uint8_t level,
	                const std::source_location& location = std::source_location::current());
	bool moveLog(const std::filesystem::path& path);
	bool isActive();

private:
	bool fileWrite(const std::string& message);
	bool cacheTransfer();
	bool directTransfer(const std::string& message);

	std::filesystem::path manifest{};
	std::fstream fileHandle{};
	std::vector<std::string> logCache{};
	LogConfiguration config{};
};
