#include "Name.h"

#include "NameTable.h"

const bool string::Name::IsEmpty() const
{
	return m_Hash == Unassigned;
}

const char* string::Name::ToChar() const
{
	return NameTable::Instance().Retrieve(m_Hash).c_str();
}

string::Name string::Name::Create(const char* string)
{
	Name name;
	name.m_Hash = NameTable::Instance().Register(string);
	return name;
}

string::Name string::Name::Create(const std::string_view& string)
{
	Name name;
	name.m_Hash = NameTable::Instance().Register(string);
	return name;
}
