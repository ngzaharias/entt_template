#include "Engine/Path.h"

str::Path::Path()
	: m_Value()
{
}

str::Path::Path(const char* string)
{
	m_Value = string;
}

str::Path::Path(const std::string_view& string)
{
	m_Value = std::string(string);
}

void str::Path::operator=(const char* string)
{
	m_Value = string;
}

void str::Path::operator=(const std::string_view& string)
{
	m_Value = std::string(string);
}

std::string_view str::Path::GetDirectory() const
{
	return m_Value;
}

std::string_view str::Path::GetFileExtension() const
{
	return m_Value;
}

std::string_view str::Path::GetFileName() const
{
	return m_Value;
}

std::string_view str::Path::GetFileNameNoExtension() const
{
	return m_Value;
}