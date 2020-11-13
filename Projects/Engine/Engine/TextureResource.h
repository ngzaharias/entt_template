#pragma once

#include <Engine/Resource.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace render
{
	struct TextureResource : public core::Resource
	{
		sf::Texture m_Texture;
	};

	class TextureLoader : public entt::resource_loader<TextureLoader, TextureResource>
	{
	public:
		std::shared_ptr<TextureResource> load(const str::Path& filepath) const
		{
			TextureResource* resource = new TextureResource();
			resource->m_Filepath = filepath;
			resource->m_Texture.loadFromFile(filepath.ToChar());
			return std::shared_ptr<TextureResource>(resource);
		}
	};

	using TextureCache = entt::resource_cache<TextureResource>;
	using TextureHandle = entt::resource_handle<TextureResource>;
}