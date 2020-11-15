#include "Engine/JsonHelpers.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>

bool json::LoadDocument(const char* filepath, rapidjson::Document& document)
{
	//load the file
	FILE* file;
	fopen_s(&file, filepath, "r");
	if (!file)
	{
		perror("fopen_s");
		fclose(file);
		return false;
	}

	constexpr size_t size = 65536;
	char* buffer = new char[size];
	rapidjson::FileReadStream is(file, buffer, size);
	rapidjson::ParseResult result = document.ParseStream(is);
	if (!result)
	{
		printf("JSON parse error: %s (%u)\n", rapidjson::GetParseError_En(result.Code()), (unsigned int)result.Offset());
		fclose(file);
		delete[] buffer;
		return false;
	}

	fclose(file);
	delete[] buffer;
	return true;
}

bool json::ParseBool(const rapidjson::Value& value, const char* member, const bool _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd(); itr->value.IsBool())
		return itr->value.GetBool();
	return _default;
}

double json::ParseDouble(const rapidjson::Value& value, const char* member, const double _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd(); itr->value.IsNumber())
		return itr->value.GetDouble();
	return _default;
}

float json::ParseFloat(const rapidjson::Value& value, const char* member, const float _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd(); itr->value.IsNumber())
		return itr->value.GetFloat();
	return _default;
}

int json::ParseInt(const rapidjson::Value& value, const char* member, const int _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd(); itr->value.IsNumber())
		return itr->value.GetInt();
	return _default;
}

const char* json::ParseString(const rapidjson::Value& value, const char* member, const char* _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd(); itr->value.IsString())
		return itr->value.GetString();
	return _default;
}

void json::PrintMembers(const rapidjson::Value& value)
{
	for (auto iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
		printf("%s\t\n", iter->name.GetString());
}
