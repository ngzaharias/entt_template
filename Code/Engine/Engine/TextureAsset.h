#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetCache.h>
#include <Engine/AssetHandle.h>
#include <Engine/AssetLoader.h>
#include <Engine/AssetPtr.h>

#include <SFML/Graphics/Texture.hpp>

namespace core
{
	struct AssetEntry;
}

namespace render
{
	struct TextureAsset : public core::Asset
	{
		sf::Texture m_Texture;
		str::Path m_SourceFile;
	};

	class TextureLoader : public core::AssetLoader<TextureLoader, TextureAsset>
	{
	public:
		bool Import(const str::Path& inputPath, const str::Path& outputPath) const;
		core::AssetPtr<TextureAsset> load(const core::AssetEntry& entry) const;
	};

	using TextureCache = core::AssetCache<TextureAsset>;
	using TextureHandle = core::AssetHandle<TextureAsset>;
}