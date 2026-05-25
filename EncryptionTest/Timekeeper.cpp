#include"Timekeeper.hpp"
using json = nlohmann::json;

Timekeeper::Timekeeper() {}

Timekeeper::Timekeeper(std::filesystem::path& fPath) : manifest{ fPath } {}

bool Timekeeper::record(const std::filesystem::path& fPath){
	std::fstream file(manifest, std::ios::out);
	json data = json::parse(file);
}

std::time_t Timekeeper::timestamp(const std::filesystem::path& fPath) {

}
