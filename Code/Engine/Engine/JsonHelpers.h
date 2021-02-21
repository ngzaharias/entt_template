#pragma once

#include <functional>
#include <rapidjson/document.h>

namespace json
{
	using Callback = std::function<bool(const rapidjson::Value&)>;

	struct Binary
	{
		bool IsValid() const { return m_Data && m_Size > 0; }

		const char* m_Data = nullptr;
		size_t m_Size = 0;
	};

	bool LoadDocument(const str::Path& filepath, rapidjson::Document& document);
	bool SaveDocument(const str::Path& filepath, rapidjson::Document& document);

	bool		ParseArray(const rapidjson::Value& value, const char* member, const json::Callback& callback);

	json::Binary	ParseBinary(const rapidjson::Value& value, const char* member, const json::Binary& _default);
	bool			ParseBool(const rapidjson::Value& value, const char* member, const bool _default);
	double			ParseDouble(const rapidjson::Value& value, const char* member, const double _default);
	template<class TEnum>
	TEnum			ParseEnum(const rapidjson::Value& value, const char* member, const TEnum _default);
	float			ParseFloat(const rapidjson::Value& value, const char* member, const float _default);
	int32			ParseInt(const rapidjson::Value& value, const char* member, const int32 _default);
	uint32			ParseUint(const rapidjson::Value& value, const char* member, const uint32 _default);
	const char*		ParseString(const rapidjson::Value& value, const char* member, const char* _default);

	void PrintMembers(const rapidjson::Value& value);
}

#include "JsonHelpers.inl"