#include "Engine/StringHelpers.h"

#include <algorithm>

bool str::Contains(std::string string, std::string substring)
{
	string = ToUpper(string);
	substring = ToUpper(substring);
	return string.find(substring) != std::string::npos;
}

bool str::ContainsAny(std::string string, const std::vector<std::string>& substrings)
{
	string = ToUpper(string);
	for (std::string substring : substrings)
	{
		substring = ToUpper(substring);
		if (string.find(substring) != std::string::npos)
			return true;
	}
	return false;
}

bool str::ContainsAll(std::string string, const std::vector<std::string>& substrings)
{
	string = ToUpper(string);
	for (std::string substring : substrings)
	{
		substring = ToUpper(substring);
		if (string.find(substring) == std::string::npos)
			return false;
	}
	return true;
}

std::vector<std::string> str::Split(const std::string& string, const std::string_view& delimiters)
{
	std::vector<std::string> substrings;
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

void str::Trim(std::string& string, const std::string_view& substring)
{
	TrimLeft(string, substring);
	TrimRight(string, substring);
}

void str::TrimLeft(std::string& string, const std::string_view& substring)
{
	const auto a_size = string.size();
	const auto b_size = substring.size();
	const auto begin = 0;
	const auto find = string.find(substring, begin);
	if (find == begin)
		string.erase(begin, b_size);
}

void str::TrimRight(std::string& string, const std::string_view& substring)
{
	const auto a_size = string.size();
	const auto b_size = substring.size();
	const auto begin = string.size() - std::min(a_size, b_size);
	const auto find = string.find(substring, begin);
	if (find == begin)
		string.erase(begin, a_size);
}

void str::TrimWhitespace(std::string& string)
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

std::string str::ToLower(std::string string)
{
	std::transform(string.begin(), string.end(), string.begin(), std::tolower);
	return string;
}

std::string str::ToUpper(std::string string)
{
	std::transform(string.begin(), string.end(), string.begin(), std::toupper);
	return string;
}
