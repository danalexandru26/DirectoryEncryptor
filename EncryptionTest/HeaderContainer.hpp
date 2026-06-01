#pragma once
#include<vector>
#include<cstdint>

class StreamWriter;

struct HeaderContainer
{
	HeaderContainer();
	HeaderContainer(uint8_t version, const std::vector<uint8_t>& iv);

	static void serialize(StreamWriter& stream, const HeaderContainer& container);

	uint8_t version;
	std::vector<uint8_t> IV;
};
