#pragma once
#include<string>
#include<vector>


class ErrorLogger {
public:
	ErrorLogger();
	ErrorLogger(const ErrorLogger& other) = delete;
	ErrorLogger& operator=(const ErrorLogger& other) = delete;

private:
	std::size_t errorCount{};
	std::vector<std::string> errorData{};
};