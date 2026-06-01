#include"HeaderContainer.hpp"

HeaderContainer::HeaderContainer()
{
}

HeaderContainer::HeaderContainer(uint8_t version, const std::vector<uint8_t>& iv) : version{version}, IV{iv}
{
}