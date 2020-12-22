#pragma once

#include <Engine/Resource.h>
#include <Engine/ResourceTypes.h>

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
		bool Import(const str::Path& inputPath, const str::Path& outputPath) const;
		std::shared_ptr<TextureResource> load(const core::ResourceEntry& entry) const;
	};

	using TextureCache = entt::resource_cache<TextureResource>;
	using TextureHandle = core::ResourceHandle<TextureResource>;
	using TexturePtr = entt::resource_handle<TextureResource>;
}