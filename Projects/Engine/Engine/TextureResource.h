#pragma once

#include <Engine/JsonHelpers.h>
#include <Engine/Resource.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace render
{
	struct TextureResource : public core::Resource
	{
		sf::Texture m_Texture;
		str::Path m_SourceFile;
	};

	class TextureLoader : public entt::resource_loader<TextureLoader, TextureResource>
	{
	public:
		std::shared_ptr<TextureResource> load(const str::Path& filepath) const
		{
			rapidjson::Document document;
			json::LoadDocument(filepath.ToChar(), document);

			str::Path sourceFile = json::ParseString(document, "source_file", nullptr);

			TextureResource* resource = new TextureResource();
			resource->m_Filepath = filepath;
			resource->m_SourceFile = sourceFile;
			resource->m_Texture.loadFromFile(sourceFile.ToChar());
			return std::shared_ptr<TextureResource>(resource);
		}
	};

	using TextureCache = entt::resource_cache<TextureResource>;
	using TextureHandle = entt::resource_handle<TextureResource>;
}