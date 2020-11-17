#pragma once

#include <Engine/String.h>
#include <Engine/StringView.h>

#include <vector>

namespace str
{
	using StringViews = std::vector<str::StringView>;

	constexpr str::StringView s_Delimiters = " .,:;'\"<>[]{}()\\|/";
	constexpr str::StringView s_Whitespace = " \t\f\v\n\r";

	bool Contains(const str::StringView& string, const str::StringView& substring);
	bool Contains_NoCase(const str::StringView& string, const str::StringView& substring);

	bool ContainsAll(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAll_NoCase(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAny(const str::StringView& string, const str::StringViews& substrings);
	bool ContainsAny_NoCase(const str::StringView& string, const str::StringViews& substrings);

	bool Equals(const str::StringView& string, const str::StringView& substring);
	bool Equals_NoCase(const str::StringView& string, const str::StringView& substring);

	str::StringViews Split(const str::StringView& string, const str::StringView& delimiters = " .,:;'\"<>[]{}()\\|/");

	void Trim(str::String& string, const str::StringView& substring);
	void TrimLeft(str::String& string, const str::StringView& substring);
	void TrimRight(str::String& string, const str::StringView& substring);
	void TrimWhitespace(str::String& string);

	str::String ToLower(str::String string);
	str::String ToUpper(str::String string);
}