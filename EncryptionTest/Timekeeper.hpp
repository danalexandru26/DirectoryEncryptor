#pragma once
#include<fstream>
#include<filesystem>
#include<string>
#include<vector>

class Timekeeper {
public:
	Timekeeper();
	Timekeeper(const Timekeeper& other) = delete;
	Timekeeper& operator=(const Timekeeper& other) = delete;

private:
	std::string manifest{};
};