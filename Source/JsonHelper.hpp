#pragma once

#include <rapidjson/document.h>

namespace JsonHelper
{
	bool LoadDocument(const char* filepath, rapidjson::Document& document);

	bool ParseBool(const rapidjson::Value& value, const char* member, bool default = false);
	double ParseDouble(const rapidjson::Value& value, const char* member, double default = 0.0);
	float ParseFloat(const rapidjson::Value& value, const char* member, float default = 0.0f);
	int ParseInt(const rapidjson::Value& value, const char* member, int default = 0);
	const char* ParseString(const rapidjson::Value& value, const char* member, const char* default = "");
}
