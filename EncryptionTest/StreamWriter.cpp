#include"StreamWriter.hpp"

void StreamWriter::writeString(const std::string& string) {
	std::size_t size = string.size();

	writeRaw(size);
	writeData(string.data(), size);
}