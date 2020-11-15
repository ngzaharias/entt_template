#include "Engine/StringHelpers.h"

#include <algorithm>

bool str::Contains(const TStringView& string, const TStringView& substring)
{
	const auto search = std::search(string.begin(), string.end(), substring.begin(), substring.end(), [](char a, char b) { return tolower(a) == tolower(b); });
	return search != string.end();
}

bool str::Contains_NoCase(const TStringView& string, const TStringView& substring)
{
	const auto search = std::search(string.begin(), string.end(), substring.begin(), substring.end());
	return search != string.end();
}

bool str::ContainsAll(const TStringView& string, const TStringViews& substrings)
{
	for (const TStringView& substring : substrings)
	{
		if (!Contains(string, substring))
			return false;
	}
	return true;
}

bool str::ContainsAll_NoCase(const TStringView& string, const TStringViews& substrings)
{
	for (const TStringView& substring : substrings)
	{
		if (!Contains_NoCase(string, substring))
			return false;
	}
	return true;
}

bool str::ContainsAny(const TStringView& string, const TStringViews& substrings)
{
	for (const TStringView& substring : substrings)
	{
		if (Contains(string, substring))
			return true;
	}
	return false;
}

bool str::ContainsAny_NoCase(const TStringView& string, const TStringViews& substrings)
{
	for (const TStringView& substring : substrings)
	{
		if (Contains_NoCase(string, substring))
			return true;
	}
	return false;
}

bool str::Equals(const TStringView& string, const TStringView& substring)
{
	return std::equal(string.begin(), string.end(), substring.begin(), substring.end());
}

bool str::Equals_NoCase(const TStringView& string, const TStringView& substring)
{
	return std::equal(string.begin(), string.end(), substring.begin(), substring.end(), [](char a, char b) { return tolower(a) == tolower(b); });
}

str::TStringViews str::Split(const TStringView& string, const TStringView& delimiters)
{
	TStringViews substrings;
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

void str::Trim(TString& string, const TStringView& substring)
{
	TrimLeft(string, substring);
	TrimRight(string, substring);
}

void str::TrimLeft(TString& string, const TStringView& substring)
{
	const auto a_size = string.size();
	const auto b_size = substring.size();
	const auto begin = 0;
	const auto find = string.find(substring, begin);
	if (find == begin)
		string.erase(begin, b_size);
}

void str::TrimRight(TString& string, const TStringView& substring)
{
	const auto a_size = string.size();
	const auto b_size = substring.size();
	const auto begin = string.size() - std::min(a_size, b_size);
	const auto find = string.find(substring, begin);
	if (find == begin)
		string.erase(begin, a_size);
}

void str::TrimWhitespace(TString& string)
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

str::TString str::ToLower(TString string)
{
	std::transform(string.begin(), string.end(), string.begin(), std::tolower);
	return string;
}

str::TString str::ToUpper(TString string)
{
	std::transform(string.begin(), string.end(), string.begin(), std::toupper);
	return string;
}
