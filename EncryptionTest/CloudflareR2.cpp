#include"CloudflareR2.hpp"

CloudFlareR2::CloudFlareR2(const std::string& keyId, const std::string& accessKey) : keyId{keyId},
	accessKey{accessKey}
{
}


void CloudFlareR2::upload(const std::string& path, std::string& cloudflareObject)
{
	std::string keyId{"d05950f1bfa47d7fc451886876f93f6d"};
	std::string secretKey{"28054c6b3939fee39f7331f4f7a9d2009919eee0f8da25b4bbc128e2faef87c3"};
	std::string endpoint{"https://7f56939aa5eaec0251aaab56888055ce.r2.cloudflarestorage.com/cryptodata/"};

	processString(cloudflareObject);

	std::string syscmd = "curl.exe -s -L --aws-sigv4 \"aws:amz:auto:s3\" "
		"--user \"" + keyId + ":" + secretKey + "\" "
		"--upload-file \"" + path + "\" " +
		endpoint + cloudflareObject;

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

void CloudFlareR2::processString(std::string& path)
{
	if (path.starts_with('/'))
	{
		path = path.substr(1);
	}

	std::size_t pos = path.find(':');
	if (pos != std::string::npos && pos + 1 < path.length())
	{
		path = path.substr(pos + 1);
	}

	std::ranges::replace(path.begin(), path.end(), '\\', '/');
}
