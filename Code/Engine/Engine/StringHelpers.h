#pragma once

#include <vector>

namespace str
{
	using StringViews = std::vector<str::StringView>;

	constexpr str::StringView strDelimiters = " .,:;'\"<>[]{}()\\|/";
	constexpr str::StringView strWhitespace = " \t\f\v\n\r";
	static const str::Name strNullGuid = NAME("00000000-0000-0000-0000-00000000");

	bool Contains(const str::StringView& string, const str::StringView& substring);
	bool Contains_NoCase(const str::StringView& string, const str::StringView& substring);

	bool ContainsAll(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAll_NoCase(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAny(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAny_NoCase(const str::StringView& string, const str::StringViews& substrings);

	bool Equals(const str::StringView& string, const str::StringView& substring);
	bool Equals_NoCase(const str::StringView& string, const str::StringView& substring);

	str::String GenerateGUID(const bool isHyphenated = true);

	str::StringViews Split(const str::StringView& string, const str::StringView& delimiters = strDelimiters);

	/// \brief Trims the substring from the left and right of the string.
	/// String: "MY STRING MY"
	/// Substring: "MY"
	/// Result: " STRING "
	void Trim(str::String& string, const str::StringView& substring);
	/// \brief Trims the substring from the left of the string.
	/// String: "MY STRING MY"
	/// Substring: "MY"
	/// Result: " STRING MY"
	void TrimLeft(str::String& string, const str::StringView& substring);
	/// \brief Trims the substring from the right of the string.
	/// String: "MY STRING MY"
	/// Substring: "MY"
	/// Result: "MY STRING "
	void TrimRight(str::String& string, const str::StringView& substring);

	/// \brief Trims all whitespace before the first and after the last non-whitespace character.
	/// String: " MY STRING " 
	/// Result: "MY STRING"
	void TrimWhitespace(str::String& string);

	/// \brief Converts all characters to their lowercase counterpart if it exists.
	/// String: "My StRiNg"
	/// Result: "my string"
	void ToLower(str::String& string);
	/// \brief Converts all characters to their uppercase counterpart if it exists.
	/// String: "My StRiNg"
	/// Result: "MY STRING" 
	void ToUpper(str::String& string);
}