#include "Engine/EnginePCH.h"
#include "Engine/Path.h"

//str::Path::Path()
//	: m_Value()
//{
//}
//
//str::Path::Path(const char* value)
//{
//	m_Value = value;
//}
//
//str::Path::Path(const str::String& value)
//{
//	m_Value = value;
//}
//
//str::Path::Path(const str::StringView& value)
//{
//	m_Value = str::String(value);
//}
//
//void str::Path::operator=(const char* rhs)
//{
//	m_Value = rhs;
//}
//
//void str::Path::operator=(const str::Path& rhs)
//{
//	m_Value = rhs.m_Value;
//}
//
//void str::Path::operator=(const str::String& rhs)
//{
//	m_Value = rhs;
//}
//
//void str::Path::operator=(const str::StringView& rhs)
//{
//	m_Value = str::String(rhs);
//}
//
//void str::Path::operator+=(const char* rhs)
//{
//	m_Value += rhs;
//}
//
//void str::Path::operator+=(const str::Path& rhs)
//{
//	m_Value += rhs.m_Value;
//}
//
//void str::Path::operator+=(const str::String& rhs)
//{
//	m_Value += rhs;
//}
//
//void str::Path::operator+=(const str::StringView& rhs)
//{
//	m_Value += str::String(rhs);
//}
//
//bool str::Path::HasFileExtension(const char* extension) const
//{
//	return m_Value.rfind(extension) != str::String::npos;
//}
//
//str::StringView str::Path::GetDirectory() const
//{
//	return m_Value;
//}
//
//str::StringView str::Path::GetFileExtension() const
//{
//	const auto find = m_Value.find_last_of('.', m_Value.size());
//	if (find != std::string::npos)
//		return { m_Value.c_str() + find, m_Value.size() - 1 };
//	return { };
//}
//
//str::StringView str::Path::GetFileName() const
//{
//	const auto find = m_Value.find_last_of("/\\", m_Value.size());
//	if (find != std::string::npos)
//		return { m_Value.c_str() + find, m_Value.size() - 1 };
//	return { };
//}
//
//str::StringView str::Path::GetFileNameNoExtension() const
//{
//	const auto findDot = m_Value.find_last_of('.', m_Value.size());
//	const auto findSlash = m_Value.find_last_of("/\\", m_Value.size());
//
//	//	/Directory/Foo.bar
//	//			  |+++|
//	if (findDot != std::string::npos && findSlash != std::string::npos)
//		return str::StringView(m_Value.c_str() + findSlash + 1, findDot - findSlash - 1);
//
//	//	Foo.bar
//	//	+++|
//	if (findDot != std::string::npos && findSlash == std::string::npos)
//		return { m_Value.c_str(), findDot };
//	return { };
//}
//
//str::StringView str::Path::GetPathNoExtension() const
//{
//	const auto find = m_Value.find_last_of('.', m_Value.size());
//	if (find != std::string::npos)
//		return { m_Value.c_str(), find };
//	return { };
//}
