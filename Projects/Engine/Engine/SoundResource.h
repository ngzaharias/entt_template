#pragma once

#include <Engine/JsonHelpers.h>
#include <Engine/Path.h>
#include <Engine/Resource.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

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
		std::shared_ptr<SoundResource> load(const str::Path& filepath) const
		{
			rapidjson::Document document;
			json::LoadDocument(filepath.ToChar(), document);

			str::Path sourceFile = json::ParseString(document, "source_file", nullptr);

			SoundResource* resource = new SoundResource();
			resource->m_Filepath = filepath;
			resource->m_SoundBuffer.loadFromFile(sourceFile.ToChar());
			resource->m_SourceFile = sourceFile;
			return std::shared_ptr<SoundResource>(resource);
		}
	};

	using SoundCache = entt::resource_cache<SoundResource>;
	using SoundHandle = entt::resource_handle<SoundResource>;
}