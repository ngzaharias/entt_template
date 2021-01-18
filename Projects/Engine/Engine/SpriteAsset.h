#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetCache.h>
#include <Engine/AssetHandle.h>
#include <Engine/AssetLoader.h>
#include <Engine/AssetPtr.h>
#include <Engine/TextureAsset.h>

#include <SFML/Graphics/Rect.hpp>

namespace core
{
	struct AssetEntry;
}

namespace render
{
	struct SpriteAsset : public core::Asset
	{
		Vector2u m_Position = { 0, 0 };
		Vector2u m_Size = { 0, 0 };
		render::TextureHandle m_Texture = { };
	};

	class SpriteLoader : public core::AssetLoader<SpriteLoader, SpriteAsset>
	{
	public:
		bool save(const SpriteAsset& asset, const core::AssetEntry& entry) const;
		core::AssetPtr<SpriteAsset> load(const core::AssetEntry& entry) const;
	};

	using SpriteCache = core::AssetCache<SpriteAsset>;
	using SpriteHandle = core::AssetHandle<SpriteAsset>;
}

REFL_AUTO
(
	type(render::SpriteAsset)
	, field(m_Position)
	, field(m_Size)
	, field(m_Texture)
)