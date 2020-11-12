#pragma once

#include <Engine/Resource.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace core
{
	struct SoundResource : public core::Resource
	{
		sf::SoundBuffer m_SoundBuffer;
	};

	class SoundLoader : public entt::resource_loader<SoundLoader, core::SoundResource>
	{
	public:
		std::shared_ptr<core::SoundResource> load(const str::Path& filepath) const
		{
			core::SoundResource* resource = new core::SoundResource();
			resource->m_Filepath = filepath;
			resource->m_SoundBuffer.loadFromFile(filepath.ToChar());
			return std::shared_ptr<core::SoundResource>(resource);
		}
	};

	using SoundCache = entt::resource_cache<core::SoundResource>;
	using SoundHandle = entt::resource_handle<core::SoundResource>;
}