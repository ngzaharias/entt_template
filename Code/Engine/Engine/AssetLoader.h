#pragma once

#include <entt/resource/loader.hpp>

namespace core
{
	template<typename Loader, typename Asset>
	using AssetLoader = entt::resource_loader<Loader, Asset>;
}