#include"ErrorLogger.hpp"

ErrorLogger::ErrorLogger() {}

void ErrorLogger::logError(const std::string& source, const std::string& error){
	record[source].push_back(error);
}

void ErrorLogger::logError(const std::vector<std::string>& sources, const std::string& error) {
	for (auto& source : sources) {
		this->logError(source, error);
	}
}

void ErrorLogger::errorRecord(const std::string& source) {
	if (record.find(source) != record.end()) {
		for (auto& error : record[source]) {
			std::cout << error << std::endl;
		}
	}
}

void ErrorLogger::errorRecord(const std::vector<std::string>& sources) {
	for (auto& source : sources) {
		this->errorRecord(source);
	}
}

int ErrorLogger::errorCount(const std::string& source) {
	if (record.find(source) != record.end()) {
		return record[source].size();
	}
	return -1;
}