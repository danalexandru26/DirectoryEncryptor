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
constexpr auto _JMETA_PATH_ = "timeMetadata.json";

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
	JMetadata(std::filesystem::path path);

	JMetadata(const JMetadata& other) = delete;
	JMetadata& operator=(const JMetadata& other) = delete;
	JMetadata& operator=(JMetadata&& other) noexcept;

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
