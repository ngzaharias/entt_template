#pragma once

#include <Engine/Resource.h>
#include <Engine/ResourceTypes.h>

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
		bool Import(const str::Path& inputPath, const str::Path& outputPath) const;
		std::shared_ptr<SoundResource> load(const core::ResourceEntry& entry) const;
	};

	using SoundCache = entt::resource_cache<SoundResource>;
	using SoundHandle = core::ResourceHandle<SoundResource>;
	using SoundPtr = entt::resource_handle<SoundResource>;
}