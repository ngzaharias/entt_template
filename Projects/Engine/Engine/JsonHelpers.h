#pragma once

#include <functional>
#include <rapidjson/document.h>

namespace json
{
	bool LoadDocument(const char* filepath, rapidjson::Document& document);

	bool		ParseBool(const rapidjson::Value& value, const char* member, const bool _default);
	double		ParseDouble(const rapidjson::Value& value, const char* member, const double _default);
	template<class TEnum>
	TEnum		ParseEnum(const rapidjson::Value& value, const char* member, const TEnum _default);
	float		ParseFloat(const rapidjson::Value& value, const char* member, const float _default);
	int			ParseInt(const rapidjson::Value& value, const char* member, const int _default);
	const char* ParseString(const rapidjson::Value& value, const char* member, const char* _default);

	void PrintMembers(const rapidjson::Value& value);
}

#include "JsonHelpers.inl"