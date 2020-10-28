#include "JsonHelper.hpp"

#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>

namespace JsonHelper
{
	bool LoadDocument(const char* filepath, rapidjson::Document& document)
	{
		//load the file
		FILE* file;
		fopen_s(&file, filepath, "r");
		if (file == nullptr)
		{
			perror("fopen_s");
			fclose(file);
			return false;
		}

		char buffer[65536];
		rapidjson::FileReadStream is(file, buffer, sizeof(buffer));
		rapidjson::ParseResult result = document.ParseStream(is);
		if (result == false)
		{
			printf("JSON parse error: %s (%u)\n", rapidjson::GetParseError_En(result.Code()), (unsigned int)result.Offset());
			fclose(file);
			return false;
		}

		fclose(file);
		return true;
	}

	bool ParseBool(const rapidjson::Value& value, const char* member, bool default)
	{
		if (value.HasMember(member) == true && value[member].IsBool() == true)
		{
			return value[member].GetBool();
		}
		return default;
	}

	double ParseDouble(const rapidjson::Value& value, const char* member, double default)
	{
		if (value.HasMember(member) == true && value[member].IsNumber() == true)
		{
			return value[member].GetDouble();
		}
		return default;
	}

	float ParseFloat(const rapidjson::Value& value, const char* member, float default)
	{
		if (value.HasMember(member) == true && value[member].IsNumber() == true)
		{
			return value[member].GetFloat();
		}
		return default;
	}

	int ParseInt(const rapidjson::Value& value, const char* member, int default)
	{
		if (value.HasMember(member) == true && value[member].IsNumber() == true)
		{
			return value[member].GetInt();
		}
		return default;
	}

	const char* ParseString(const rapidjson::Value& value, const char* member, const char* default)
	{
		if (value.HasMember(member) == true && value[member].IsBool() == true)
		{
			return value[member].GetString();
		}
		return default;
	}
}