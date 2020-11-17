#pragma once

#include <Engine/JsonHelpers.h>
#include <Engine/Resource.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace core
{
	struct ResourceEntry;
}

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
		std::shared_ptr<TextureResource> load(const core::ResourceEntry& resourceEntry) const;
	};

	using TextureCache = entt::resource_cache<TextureResource>;
	using TextureHandle = entt::resource_handle<TextureResource>;
}