#pragma once

#include <Engine/Resource.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace core
{
	struct TextureResource : public core::Resource
	{
		sf::Texture m_Texture;
	};

	class TextureLoader : public entt::resource_loader<TextureLoader, core::TextureResource>
	{
	public:
		std::shared_ptr<core::TextureResource> load(const str::Path& filepath) const
		{
			core::TextureResource* resource = new core::TextureResource();
			resource->m_Filepath = filepath;
			resource->m_Texture.loadFromFile(filepath.ToChar());
			return std::shared_ptr<core::TextureResource>(resource);
		}
	};

	using TextureCache = entt::resource_cache<core::TextureResource>;
	using TextureHandle = entt::resource_handle<core::TextureResource>;
}