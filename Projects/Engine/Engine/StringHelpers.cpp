#include "Engine/StringHelpers.h"

#include <algorithm>

bool str::Contains(const str::StringView& string, const str::StringView& substring)
{
	const auto search = std::search(string.begin(), string.end(), substring.begin(), substring.end(), [](char a, char b) { return tolower(a) == tolower(b); });
	return search != string.end();
}

bool str::Contains_NoCase(const str::StringView& string, const str::StringView& substring)
{
	const auto search = std::search(string.begin(), string.end(), substring.begin(), substring.end());
	return search != string.end();
}

bool str::ContainsAll(const str::StringView& string, const str::StringViews& substrings)
{
	for (const str::StringView& substring : substrings)
	{
		if (!Contains(string, substring))
			return false;
	}
	return true;
}

bool str::ContainsAll_NoCase(const str::StringView& string, const str::StringViews& substrings)
{
	for (const str::StringView& substring : substrings)
	{
		if (!Contains_NoCase(string, substring))
			return false;
	}
	return true;
}

bool str::ContainsAny(const str::StringView& string, const str::StringViews& substrings)
{
	for (const str::StringView& substring : substrings)
	{
		if (Contains(string, substring))
			return true;
	}
	return false;
}

bool str::ContainsAny_NoCase(const str::StringView& string, const str::StringViews& substrings)
{
	for (const str::StringView& substring : substrings)
	{
		if (Contains_NoCase(string, substring))
			return true;
	}
	return false;
}

bool str::Equals(const str::StringView& string, const str::StringView& substring)
{
	return std::equal(string.begin(), string.end(), substring.begin(), substring.end());
}

bool str::Equals_NoCase(const str::StringView& string, const str::StringView& substring)
{
	return std::equal(string.begin(), string.end(), substring.begin(), substring.end(), [](char a, char b) { return tolower(a) == tolower(b); });
}

str::StringViews str::Split(const str::StringView& string, const str::StringView& delimiters)
{
	str::StringViews substrings;
	auto begin = string.find_first_not_of(delimiters, 0);
	auto end = string.find_first_of(delimiters, begin);
	while (begin != std::string::npos || end != std::string::npos)
	{
		substrings.push_back(string.substr(begin, end - begin));
		begin = string.find_first_not_of(delimiters, end);
		end = string.find_first_of(delimiters, begin);
	}
	return substrings;
}

void str::Trim(str::String& string, const str::StringView& substring)
{
	TrimLeft(string, substring);
	TrimRight(string, substring);
}

void str::TrimLeft(str::String& string, const str::StringView& substring)
{
	const auto a_size = string.size();
	const auto b_size = substring.size();
	const auto begin = 0;
	const auto find = string.find(substring, begin);
	if (find == begin)
		string.erase(begin, b_size);
}

void str::TrimRight(str::String& string, const str::StringView& substring)
{
	const auto a_size = string.size();
	const auto b_size = substring.size();
	const auto begin = string.size() - std::min(a_size, b_size);
	const auto find = string.find(substring, begin);
	if (find == begin)
		string.erase(begin, a_size);
}

void str::TrimWhitespace(str::String& string)
{
	const auto begin = string.find_first_not_of(s_Whitespace, 0);
	const auto end = string.find_last_not_of(s_Whitespace) + 1;
	if (begin != std::string::npos && end != std::string::npos)
	{
		string = string.substr(begin, end - begin);
	}
	else
	{
		string.clear();
	}
}

str::String str::ToLower(str::String string)
{
	std::transform(string.begin(), string.end(), string.begin(), std::tolower);
	return string;
}

str::String str::ToUpper(str::String string)
{
	std::transform(string.begin(), string.end(), string.begin(), std::toupper);
	return string;
}
