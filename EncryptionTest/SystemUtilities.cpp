#include"SystemUtilities.hpp"

const char* readEnvironmentVariable(const std::string& variable)
{
	char* buffer{nullptr};
	std::size_t size{};

	_dupenv_s(&buffer, &size, variable.data());

	return buffer;
}
