#pragma once

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>

namespace core
{
	struct Asset
	{
		str::Name m_Guid;
		str::Path m_Filepath;
	};
}