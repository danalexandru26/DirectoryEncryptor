#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include<vector>
#include<chrono>
#include <nlohmann/json.hpp>

constexpr auto _JMETA_TIMESTAMP_ = "Timestamp";
constexpr auto _JMETA_FILESIZE_ = "Filesize";
constexpr auto _JMETA_HASH_ = "Hash/Checksum";

using json = nlohmann::json;

enum JSONKeys
{
	Timestamp,
	Filesize,
	Hash
};

class JMetadata
{
public:
	JMetadata();
	JMetadata(const std::filesystem::path& fPath);
	JMetadata(const JMetadata& other) = delete;
	JMetadata& operator=(const JMetadata& other) = delete;

	void record(const std::filesystem::path& path, const JSONKeys& key);
	void record(const std::filesystem::path& path, const std::vector<JSONKeys>& keys);
	bool saveJM();

private:
	std::time_t computeTimestamp(const std::filesystem::path& path);
	uintmax_t computeFilesize(const std::filesystem::path& path);
private:
	json data{};
	std::filesystem::path manifest{};
};
