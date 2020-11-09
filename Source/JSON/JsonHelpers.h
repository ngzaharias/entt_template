#pragma once

#include <functional>
#include <rapidjson/document.h>

namespace json
{
	using TCallback = std::function<void(rapidjson::Value& value)>;

	bool LoadDocument(const char* filepath, rapidjson::Document& document);

	bool		ParseBool(const rapidjson::Value& value, const char* member, const bool default);
	double		ParseDouble(const rapidjson::Value& value, const char* member, const double default);
	float		ParseFloat(const rapidjson::Value& value, const char* member, const float default);
	int			ParseInt(const rapidjson::Value& value, const char* member, const int default);
	const char* ParseString(const rapidjson::Value& value, const char* member, const char* default);

	void PrintMembers(const rapidjson::Value& value);
}
