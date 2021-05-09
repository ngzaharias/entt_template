#pragma once

#include <Engine/String.h>

#include <entt/fwd.hpp>
#include <entt/core/hashed_string.hpp>

namespace core
{
	using Hash = entt::id_type;
}

namespace str
{
	core::Hash ToHash(const char* value);
	core::Hash ToHash(const str::String& value);
}