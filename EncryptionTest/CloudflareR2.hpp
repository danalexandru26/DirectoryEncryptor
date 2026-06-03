#pragma once
#include<iostream>
#include<cstdlib>
#include <filesystem>
#include<string>

class CloudFlareR2
{
public:
	CloudFlareR2() = default;
	CloudFlareR2(const std::string& keyId, const std::string& accessKey);

	int interrogate();
	void upload(const std::string& path, std::string cloudflareObject);

private:
	void processString(std::string& path);

	std::string keyId;
	std::string accessKey;
};
