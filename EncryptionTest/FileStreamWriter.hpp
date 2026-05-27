#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include<exception>

#include"StreamWriter.hpp"

class FileStreamWriter : public StreamWriter
{
public:
	FileStreamWriter(std::filesystem::path& path);
	~FileStreamWriter() override;

	FileStreamWriter(const FileStreamWriter& other) = delete;
	FileStreamWriter(FileStreamWriter&& other) noexcept;

	FileStreamWriter& operator=(const FileStreamWriter& other) = delete;
	FileStreamWriter& operator=(FileStreamWriter&& other) noexcept;

	bool writeData(const char* data, std::size_t size) final;
	void writeString(const std::string& data) final;

private:
	void moveData(FileStreamWriter& other);

private:
	std::filesystem::path mPath{};
	std::ofstream file{};
};
