#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetCache.h>
#include <Engine/AssetHandle.h>
#include <Engine/AssetLoader.h>
#include <Engine/AssetPtr.h>
#include <Engine/AttributeTypes.h>
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
		SpriteHandle m_Sprite = { };
		int32 m_FrameCount = 1;
	};

	struct FlipbookAsset : public core::Asset
	{
		std::vector<FlipbookFrame> m_Frames = { };
		float m_FPS = 30.f;
		bool m_IsLooping = true;
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
	, field(m_FPS, attr::Range(1.f, 100.f))
	, field(m_Frames)
	, field(m_IsLooping)
)