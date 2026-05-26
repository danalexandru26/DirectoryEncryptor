#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include<vector>
#include<chrono>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum JSONKeys
{
	Timestamp,
	Filesize,
	SHA256
};

class JMetadata
{
public:
	JMetadata();
	JMetadata(const std::filesystem::path& fPath);
	JMetadata(const JMetadata& other) = delete;
	JMetadata& operator=(const JMetadata& other) = delete;

	void record(const std::filesystem::path& fPath);
	void record(const std::filesystem::path& path, const std::vector<JSONKeys>& keys);
	bool saveJM();

private:
	std::time_t computeTimestamp(const std::filesystem::path& fPath);

private:
	json data{};
	std::filesystem::path manifest{};
};
