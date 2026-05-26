#pragma once
#include<iostream>
#include<filesystem>
#include<fstream>
#include<system_error>
#include<string>
#include<format>

#define _DEFAULT_LOG_PATH_ "runtime_log.json"
constexpr uint16_t _LOG_CHUNK_ = 1024;

enum Severity
{
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

class Log
{
public:
	Log();
	virtual ~Log();
	Log(const Log& other) = delete;
	Log& operator=(const Log& other) = delete;

	bool logMessage(const std::string& message);
	bool moveLog(const std::filesystem::path& path);

private:
	bool initFileHandle();

private:
	std::fstream fileHandle{};
	std::filesystem::path manifest{};

	std::vector<std::string> logCache{};
};
