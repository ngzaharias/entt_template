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

str::Path::Path(const str::StringView& string)
{
	m_Value = str::String(string);
}

void str::Path::operator=(const char* string)
{
	m_Value = string;
}

void str::Path::operator=(const str::StringView& string)
{
	m_Value = str::String(string);
}

bool str::Path::HasFileExtension(const char* extension) const
{
	return m_Value.rfind(extension) != str::String::npos;
}

str::StringView str::Path::GetDirectory() const
{
	return m_Value;
}

str::StringView str::Path::GetFileExtension() const
{
	const auto find = m_Value.find_last_of('.', m_Value.size());
	if (find != std::string::npos)
		return { m_Value.c_str() + find, m_Value.size() - 1 };
	return { };
}

str::StringView str::Path::GetFileName() const
{
	return m_Value;
}

str::StringView str::Path::GetFileNameNoExtension() const
{
	return m_Value;
}