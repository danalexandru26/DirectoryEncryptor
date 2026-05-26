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
		this->record(path, key);
	}
}

void JMetadata::record(const std::filesystem::path& path, const JSONKeys& key)
{
	switch (key)
	{
	case JSONKeys::Timestamp:
		data[path.generic_string()][_JMETA_TIMESTAMP_] = this->computeTimestamp(path);
		break;
	case JSONKeys::Filesize:
		data[path.generic_string()][_JMETA_FILESIZE_] = this->computeFilesize(path);
		break;
	case JSONKeys::Hash:
		std::cerr << "File hash and checksums are not supported yet\n";
		break;
	default:
		std::cerr << "Invalid JSON object key\n";
		break;
	}
}

bool JMetadata::saveJM()
{
	std::ofstream file(manifest);

	if (!file)
	{
		std::cerr << "Manifest file cannot be opened or created\n";
		return false;
	}
	file << std::setw(4) << data << '\n';

	file.close();
	return true;
}

std::time_t JMetadata::computeTimestamp(const std::filesystem::path& path)
{
	std::filesystem::file_time_type fileTime = std::filesystem::last_write_time(path);
	std::chrono::system_clock::time_point systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fileTime);

	return std::chrono::system_clock::to_time_t(systemTime);
}

uintmax_t JMetadata::computeFilesize(const std::filesystem::path& path)
{
	return std::filesystem::file_size(path);
}

