#include "EnginePCH.h"
#include "Engine/Hash.h"


// #todo: add str::StringView version

core::Hash str::ToHash(const char* value)
{
	return entt::hashed_string(value);
}

core::Hash str::ToHash(const str::String& value)
{
	return entt::hashed_string(value.c_str());
}