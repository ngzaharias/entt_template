#include "EnginePCH.h"
#include "Engine/NameTable.h"

#include <assert.h>

namespace
{
	str::String strEmpty;

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

core::Hash str::NameTable::Register(const char* string)
{
	const core::Hash hash = str::ToHash(string);
	const auto itr = m_Values.find(hash);
	if (itr == m_Values.end())
		m_Values[hash] = string;
	return hash;
}

core::Hash str::NameTable::Register(const str::String& string)
{
	const core::Hash hash = str::ToHash(string);
	const auto itr = m_Values.find(hash);
	if (itr == m_Values.end())
		m_Values[hash] = string;
	return hash;
}

core::Hash str::NameTable::Register(const str::StringView& string)
{
	// #fixme: shouldn't need to convert to string
	const core::Hash hash = str::ToHash(str::String(string));
	const auto itr = m_Values.find(hash);
	if (itr == m_Values.end())
		m_Values[hash] = string;
	return hash;
}

const str::String& str::NameTable::Retrieve(const core::Hash& hash) const
{
	const auto itr = m_Values.find(hash);
	if (itr != m_Values.end())
		return itr->second;
	return strEmpty;
}
