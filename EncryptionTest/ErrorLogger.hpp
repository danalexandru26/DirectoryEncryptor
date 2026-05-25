#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>


class ErrorLogger {
public:
	ErrorLogger();
	ErrorLogger(const ErrorLogger& other) = delete;
	ErrorLogger& operator=(const ErrorLogger& other) = delete;

	void logError(const std::string& source, const std::string& error);
	void logError(const std::vector<std::string>& sources, const std::string& error);
	void errorRecord(const std::string& source);
	void errorRecord(const std::vector<std::string>& sources);
	int errorCount(const std::string& source);

private:
	std::unordered_map<std::string, std::vector<std::string>> record{};
};