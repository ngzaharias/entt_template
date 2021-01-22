#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetCache.h>
#include <Engine/AssetHandle.h>
#include <Engine/AssetLoader.h>
#include <Engine/AssetPtr.h>
#include <Engine/SpriteAsset.h>

#include <SFML/Graphics/Texture.hpp>

namespace core
{
	struct AssetEntry;
}

namespace render
{
	struct FlipbookFrame
	{
		int32 m_FrameCount = 0;
		SpriteHandle m_Sprite = { };
	};

	struct FlipbookAsset : public core::Asset
	{
		float m_FPS = 0.f;
		bool m_IsLooping = true;
		std::vector<FlipbookFrame> m_Frames = { };
	};

	class FlipbookLoader : public core::AssetLoader<FlipbookLoader, FlipbookAsset>
	{
	public:
		bool save(const FlipbookAsset& asset, const core::AssetEntry& entry) const;
		core::AssetPtr<FlipbookAsset> load(const core::AssetEntry& entry) const;
	};

	using FlipbookCache = core::AssetCache<FlipbookAsset>;
	using FlipbookHandle = core::AssetHandle<FlipbookAsset>;
}

REFL_AUTO
(
	type(render::FlipbookFrame)
	, field(m_FrameCount)
	, field(m_Sprite)
)

REFL_AUTO
(
	type(render::FlipbookAsset)
	, field(m_FPS)
	, field(m_Frames)
)