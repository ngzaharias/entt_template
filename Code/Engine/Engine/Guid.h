#pragma once

#include <Engine/Name.h>

#define GUID(string) str::Guid::Create(string)

namespace str
{
	using Guid = str::Name;
}