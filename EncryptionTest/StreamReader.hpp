#pragma once
#include <cstddef>
#include <cstdint>
#include<vector>
#include<string>

class StreamReader
{
public:
	StreamReader() = default;

	StreamReader(const StreamReader& other) = delete;
	StreamReader& operator=(const StreamReader& other) = delete;

	virtual const char* readData(char* destination, std::size_t size) = 0;
	virtual void readString(std::string& string);


	template <typename T>
	void readRaw(const T& type)
	{
		readData(type, sizeof(type));
	}

	template <typename T>
	void readArray(std::vector<T>& type, uint32_t size = 0)
	{
	}

	template <typename T>
	void readObjectA(T type)
	{
		T::deserialize(this, type);
	}
};
