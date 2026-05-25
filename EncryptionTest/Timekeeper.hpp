#pragma once
#include<fstream>
#include<filesystem>
#include<string>
#include<vector>
#include<chrono>
#include <nlohmann/json.hpp>

class Timekeeper {
public:
	Timekeeper();
	Timekeeper(std::filesystem::path& fPath);
	Timekeeper(const Timekeeper& other) = delete;
	Timekeeper& operator=(const Timekeeper& other) = delete;

	bool record(const std::filesystem::path& fPath);
	std::time_t timestamp(const std::filesystem::path& fPath);

private:
	std::filesystem::path manifest{};
};
