#pragma once
#include<fstream>
#include<string>
#include<vector>


class StreamWriter
{
public:
	virtual ~StreamWriter() = default;

	virtual bool writeData(const char* data, std::size_t size) = 0;
	virtual void writeString(const std::string& data) = 0;

	template <typename T>
	void writeRaw(const T& type)
	{
		writeData(reinterpret_cast<const char*>(&type), sizeof(T));
	}

	template <typename T>
	void writeObject(const T& type)
	{
		if constexpr (std::is_trivial<T>())
		{
			writeRaw(type);
		}
		else T::serialize(*this, type);
	}

	template <typename T>
	void writeArray(const std::vector<T>& type)
	{
		for (const auto& i : type)
		{
			if constexpr (std::is_trivial<T>())
			{
				writeRaw(i);
			}
			else writeObject(i);
		}
	}
};
