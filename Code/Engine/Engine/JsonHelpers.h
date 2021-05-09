#pragma once

#include <Engine/JsonTypes.h>

#include <functional>
#include <rapidjson/document.h>

namespace json
{
	using Callback = std::function<bool(const json::Object&)>;

	struct Binary
	{
		bool IsValid() const { return m_Data && m_Size > 0; }

		const char* m_Data = nullptr;
		size_t m_Size = 0;
	};

	bool LoadDocument(const str::Path& filepath, json::Document& document);
	bool SaveDocument(const str::Path& filepath, json::Document& document);

	/// \brief 
	bool ParseArray(const json::Object& value, const char* member, const json::Callback& callback);

	/// \brief Parses a json::Object into a binary format and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	json::Binary ParseBinary(const json::Object& value, const char* member, const json::Binary& _default);

	/// \brief Parses a json::Object into a bool and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	bool ParseBool(const json::Object& value, const char* member, const bool _default);

	/// \brief Parses a json::Object into a double and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	double ParseDouble(const json::Object& value, const char* member, const double _default);

	/// \brief Parses a json::Object into a specified enum and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	template<class TEnum>
	TEnum ParseEnum(const json::Object& value, const char* member, const TEnum _default);

	/// \brief Parses a json::Object into a float and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	float ParseFloat(const json::Object& value, const char* member, const float _default);

	/// \brief Parses a json::Object into an integer and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	int32 ParseInt(const json::Object& value, const char* member, const int32 _default);

	/// \brief Parses a json::Object into an unsigned integer and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	uint32 ParseUint(const json::Object& value, const char* member, const uint32 _default);

	/// \brief Parses a json::Object into a string and returns it.
	/// If the parsing fails for any reason the _default value will be returned instead.
	str::String ParseString(const json::Object& value, const char* member, const str::String& _default);

	/// \brief Helper method that prints all members of a json::Object to console.
	void PrintMembers(const json::Object& value);
}

#include "JsonHelpers.inl"