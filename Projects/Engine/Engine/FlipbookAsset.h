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
	struct FlipbookAsset : public core::Asset
	{
		str::Name m_TextureGuid;
		int32 m_SpriteCount = 0;
		Vector2i m_SpriteSize = { 0, 0 };
	};

	class FlipbookLoader : public core::AssetLoader<FlipbookLoader, FlipbookAsset>
	{
	public:
		bool create(const core::AssetEntry& entry) const;
		core::AssetPtr<FlipbookAsset> load(const core::AssetEntry& entry) const;
	};

	using FlipbookCache = core::AssetCache<FlipbookAsset>;
	using FlipbookHandle = core::AssetHandle<FlipbookAsset>;
}