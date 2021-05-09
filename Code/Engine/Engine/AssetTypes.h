#pragma once

#include <Engine/TypeId.h>

#include <entt/core/type_info.hpp>

namespace audio
{
	struct SoundAsset;
}

namespace physics
{
	struct MaterialAsset;
}

namespace render
{
	struct FlipbookAsset;
	struct SpriteAsset;
	struct TextureAsset;
}

namespace core
{
	struct TemplateAsset;

	enum class EAssetType : entt::id_type
	{
		Unknown,

		Flipbook			= core::ToTypeId<render::FlipbookAsset>(),
		PhysicsMaterial		= core::ToTypeId<physics::MaterialAsset>(),
		Sound				= core::ToTypeId<audio::SoundAsset>(),
		Sprite				= core::ToTypeId<render::SpriteAsset>(),
		Template			= core::ToTypeId<core::TemplateAsset>(),
		Texture				= core::ToTypeId<render::TextureAsset>(),
	};

	struct AssetEntry
	{
		core::EAssetType	m_Type = EAssetType::Unknown;
		str::Name			m_Guid = { };
		str::Path			m_Filepath = { };
	};
}