#pragma once

#include <entt/resource/handle.hpp>

namespace core
{
	template<typename Type>
	using AssetHandle = entt::resource_handle<Type>;
}