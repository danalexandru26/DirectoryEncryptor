#include"JMetadata.hpp"
using json = nlohmann::json;

JMetadata::JMetadata()
{
	manifest = "timeMetadata.json";
}

JMetadata::JMetadata(const std::filesystem::path& fPath) : manifest{fPath}
{
}

void JMetadata::record(const std::filesystem::path& path, const std::vector<JSONKeys>& keys)
{
	for (JSONKeys key : keys)
	{
		
	}
}

void JMetadata::record(const std::filesystem::path& fPath)
{
	std::time_t lastChanged = this->computeTimestamp(fPath);
	std::uintmax_t fileSize = std::filesystem::file_size(fPath);

	data[fPath.string()]["Timestamp"] = lastChanged;
	data[fPath.string()]["Bytes"] = fileSize;
}

bool JMetadata::saveJM()
{
	std::ofstream file(manifest);

	if (!file)
	{
		std::cerr << "Manifest file cannot be opened or created" << std::endl;
		return false;
	}
	file << std::setw(4) << data << std::endl;

	file.close();
	return true;
}

std::time_t JMetadata::computeTimestamp(const std::filesystem::path& fPath)
{
	std::filesystem::file_time_type fileTime = std::filesystem::last_write_time(fPath);
	std::chrono::system_clock::time_point systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fileTime);

	return std::chrono::system_clock::to_time_t(systemTime);
}
