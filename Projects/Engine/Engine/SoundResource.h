#pragma once

#include <Engine/Path.h>
#include <Engine/Resource.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace core
{
	struct ResourceEntry;
}

namespace audio
{
	struct SoundResource : public core::Resource
	{
		sf::SoundBuffer m_SoundBuffer;
		str::Path m_SourceFile;
	};

	class SoundLoader : public entt::resource_loader<SoundLoader, SoundResource>
	{
	public:
		std::shared_ptr<SoundResource> load(const core::ResourceEntry& resourceEntry) const;
	};

	using SoundCache = entt::resource_cache<SoundResource>;
	using SoundHandle = entt::resource_handle<SoundResource>;
}