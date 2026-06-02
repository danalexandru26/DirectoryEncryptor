#include"CloudflareR2.hpp"

void uploadToCloudflareR2(const std::string& path, std::string cloudflareFilename)
{
	std::string keyId{"d05950f1bfa47d7fc451886876f93f6d"};
	std::string secretKey{"28054c6b3939fee39f7331f4f7a9d2009919eee0f8da25b4bbc128e2faef87c3"};
	std::string endpoint{"https://7f56939aa5eaec0251aaab56888055ce.r2.cloudflarestorage.com/cryptodata/"};

	std::size_t div = cloudflareFilename.find(":");
	if (div != std::string::npos)
	{
		cloudflareFilename = cloudflareFilename.substr(div + 1);
	}
	std::replace(cloudflareFilename.begin(), cloudflareFilename.end(), '\\', '/');
	
	if (cloudflareFilename.starts_with('/') == 0)
	{
		cloudflareFilename = cloudflareFilename.substr(1);
	}
	
	std::cout << cloudflareFilename << '\n';

	std::string syscmd = "curl.exe -s -L --aws-sigv4 \"aws:amz:auto:s3\" "
		"--user \"" + keyId + ":" + secretKey + "\" "
		"--upload-file \"" + path + "\" " +
		endpoint + cloudflareFilename;

	int op = std::system(syscmd.c_str());

	if (!op)
	{
		std::cerr << "Cloud upload is successful\n";
	}
	else
	{
		std::cerr << "Cloudflare upload failure\n";
	}
}
