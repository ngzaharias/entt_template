#include "Name.h"

#include "NameTable.h"

const bool str::Name::IsEmpty() const
{
	return m_Hash == Unassigned;
}

const char* str::Name::ToChar() const
{
	return NameTable::Instance().Retrieve(m_Hash).c_str();
}

const std::string& str::Name::ToString() const
{
	return NameTable::Instance().Retrieve(m_Hash);
}

str::Name str::Name::Create(const char* string)
{
	Name name;
	name.m_Hash = NameTable::Instance().Register(string);
	return name;
}

str::Name str::Name::Create(const std::string_view& string)
{
	Name name;
	name.m_Hash = NameTable::Instance().Register(string);
	return name;
}
