#pragma once

#include <entt/resource/cache.hpp>

namespace core
{
	template<typename Type>
	using AssetCache = entt::resource_cache<Type>;
}
