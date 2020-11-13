#pragma once

#include <functional>
#include <rapidjson/document.h>

namespace json
{
	using TCallback = std::function<void(rapidjson::Value& value)>;

	bool LoadDocument(const char* filepath, rapidjson::Document& document);

	bool		ParseBool(const rapidjson::Value& value, const char* member, const bool dflt);
	double		ParseDouble(const rapidjson::Value& value, const char* member, const double dflt);
	float		ParseFloat(const rapidjson::Value& value, const char* member, const float dflt);
	int			ParseInt(const rapidjson::Value& value, const char* member, const int dflt);
	const char* ParseString(const rapidjson::Value& value, const char* member, const char* dflt);

	void PrintMembers(const rapidjson::Value& value);
}
