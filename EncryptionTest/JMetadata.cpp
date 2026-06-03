#include"JMetadata.hpp"
using json = nlohmann::json;

JMetadata::JMetadata()
{
	manifest = _JMETA_PATH_;
}

JMetadata::JMetadata(std::filesystem::path path)
{
	manifest = std::move(path);
}

JMetadata& JMetadata::operator=(JMetadata&& other) noexcept
{
	if (this != &other)
	{
		data = std::move(other.data);
		manifest = std::move(other.manifest);
	}
	return *this;
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
		break;
	default:
		break;
	}
}

void JMetadata::record(const std::filesystem::path& path, const std::string& key, const std::string& value)
{
	data[path.generic_string()][key] = value;
}

void JMetadata::record(const std::filesystem::path& path, const std::vector<JSONKeys>& keys)
{
	for (JSONKeys key : keys)
	{
		this->record(path, key);
	}
}

bool JMetadata::check(const std::filesystem::path& path, const std::string& key, const std::string& value)
{
	if (data.contains(path.generic_string()))
	{
		for (auto& [entry, val] : data[path.generic_string()].items())
		{
			if (entry == key)return val == value;
		}
	}
	return false;
}

void JMetadata::saveJM()
{
	std::ofstream file(manifest);

	if (!file.is_open())
	{
		std::cerr << "Manifest file cannot be opened or created\n";
	}
	file << std::setw(4) << data << '\n';

	file.close();
}

void JMetadata::extractJM()
{
	std::ifstream file(manifest);

	try
	{
		data = nlohmann::json::parse(file);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
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
