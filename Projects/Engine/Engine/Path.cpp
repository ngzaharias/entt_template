#include "Engine/EnginePCH.h"
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

bool str::Path::HasFileExtension(const char* extension) const
{
	return m_Value.rfind(extension) != std::string::npos;
}

std::string_view str::Path::GetDirectory() const
{
	return m_Value;
}

std::string_view str::Path::GetFileExtension() const
{
	const auto find = m_Value.find_last_of('.', m_Value.size());
	if (find != std::string::npos)
		return { m_Value.c_str() + find, m_Value.size() - 1 };
	return { };
}

std::string_view str::Path::GetFileName() const
{
	return m_Value;
}

std::string_view str::Path::GetFileNameNoExtension() const
{
	return m_Value;
}