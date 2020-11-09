#include "NameTable.hpp"

#include <assert.h>

namespace
{
	std::string strEmpty;

	string::NameTable* s_Instance = nullptr;
}

string::NameTable& string::NameTable::Instance()
{
	if (!s_Instance)
		s_Instance = new NameTable();
	return *s_Instance;
}

string::NameTable::NameTable()
{
	s_Instance = this;
}

string::NameTable::~NameTable()
{
	s_Instance = nullptr;
}

string::Hash string::NameTable::Register(const char* string)
{
	const std::string value = string;
	const Hash hash = std::hash<std::string>{}(value);
	const auto itr = m_Strings.find(hash);
	if (itr == m_Strings.end())
		m_Strings[hash] = string;
	return hash;
}

string::Hash string::NameTable::Register(const std::string_view& string)
{
	const std::string value = std::string(string);
	const Hash hash = std::hash<std::string>{}(value);
	const auto itr = m_Strings.find(hash);
	if (itr != m_Strings.end())
		m_Strings[hash] = string;
	return hash;
}

const std::string& string::NameTable::Retrieve(const Hash& hash) const
{
	const auto itr = m_Strings.find(hash);
	if (itr != m_Strings.end())
		return itr->second;
	return strEmpty;
}

