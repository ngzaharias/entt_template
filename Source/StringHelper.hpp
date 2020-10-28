#pragma once

#include <string>
#include <vector>

namespace string
{
	constexpr char* delimiters = " .,:;'\"<>[]{}()\\|/";
	constexpr char* whitespace = " \t\f\v\n\r";

	bool Contains(std::string string, std::string substring);
	
	bool ContainsAny(std::string string, const std::vector<std::string>& substrings);
	bool ContainsAll(std::string string, const std::vector<std::string>& substrings);

	std::vector<std::string> Split(const std::string& string, const std::string_view& delimiters = " .,:;'\"<>[]{}()\\|/");
	
	void Trim(std::string& string, const std::string_view& substring);
	void TrimLeft(std::string& string, const std::string_view& substring);
	void TrimRight(std::string& string, const std::string_view& substring);
	void TrimWhitespace(std::string& string);

	std::string ToLower(std::string string);
	std::string ToUpper(std::string string);
}