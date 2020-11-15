#include "Engine/Guid.h"

str::Guid::Guid()
{
}

str::Guid::Guid(const char* string)
{
	m_Value = entt::hashed_string{ string };
}

str::Guid::Guid(const std::string_view& string)
{
	m_Value = entt::hashed_string{ std::string(string).c_str() };
}