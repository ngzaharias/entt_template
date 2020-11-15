#pragma once

template<class TEnum>
TEnum json::ParseEnum(const rapidjson::Value& value, const char* member, const TEnum _default)
{
	const auto itr = value.FindMember(member);
	if (itr != value.MemberEnd(); itr->value.IsNumber())
		return static_cast<TEnum>(itr->value.GetInt());
	return _default;
}