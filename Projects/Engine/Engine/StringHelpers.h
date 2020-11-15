#pragma once

#include <string>
#include <vector>

namespace str
{
	using TString = std::string;
	using TStringView = std::string_view;
	using TStringViews = std::vector<TStringView>;

	constexpr std::string_view s_Delimiters = " .,:;'\"<>[]{}()\\|/";
	constexpr std::string_view s_Whitespace = " \t\f\v\n\r";

	bool Contains(const TStringView& string, const TStringView& substring);
	bool Contains_NoCase(const TStringView& string, const TStringView& substring);

	bool ContainsAll(const TStringView& string, const TStringViews& substrings);
	bool ContainsAll_NoCase(const TStringView& string, const TStringViews& substrings);
	bool ContainsAny(const TStringView& string, const TStringViews& substrings);
	bool ContainsAny_NoCase(const TStringView& string, const TStringViews& substrings);

	bool Equals(const TStringView& string, const TStringView& substring);
	bool Equals_NoCase(const TStringView& string, const TStringView& substring);

	TStringViews Split(const TStringView& string, const TStringView& delimiters = " .,:;'\"<>[]{}()\\|/");

	void Trim(TString& string, const TStringView& substring);
	void TrimLeft(TString& string, const TStringView& substring);
	void TrimRight(TString& string, const TStringView& substring);
	void TrimWhitespace(TString& string);

	TString ToLower(TString string);
	TString ToUpper(TString string);
}