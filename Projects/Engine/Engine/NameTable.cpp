#include "Engine/NameTable.h"

#include <assert.h>

namespace
{
	std::string strEmpty;

	str::NameTable* s_Instance = nullptr;
}

str::NameTable& str::NameTable::Instance()
{
	if (!s_Instance)
		s_Instance = new NameTable();
	return *s_Instance;
}

str::NameTable::NameTable()
{
	s_Instance = this;
}

str::NameTable::~NameTable()
{
	s_Instance = nullptr;
}

str::Hash str::NameTable::Register(const char* string)
{
	const std::string value = string;
	const Hash hash = std::hash<std::string>{}(value);
	const auto itr = m_Values.find(hash);
	if (itr == m_Values.end())
		m_Values[hash] = string;
	return hash;
}

str::Hash str::NameTable::Register(const std::string_view& string)
{
	const std::string value = std::string(string);
	const Hash hash = std::hash<std::string>{}(value);
	const auto itr = m_Values.find(hash);
	if (itr != m_Values.end())
		m_Values[hash] = string;
	return hash;
}

const std::string& str::NameTable::Retrieve(const Hash& hash) const
{
	const auto itr = m_Values.find(hash);
	if (itr != m_Values.end())
		return itr->second;
	return strEmpty;
}

