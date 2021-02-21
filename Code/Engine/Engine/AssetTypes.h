#pragma once

#include <entt/fwd.hpp>
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
	struct EntityTemplateAsset;

	enum class EAssetType : entt::id_type
	{
		Unknown,

		EntityTemplate		= entt::type_info<core::EntityTemplateAsset>::id(),
		Flipbook			= entt::type_info<render::FlipbookAsset>::id(),
		PhysicsMaterial		= entt::type_info<physics::MaterialAsset>::id(),
		Sound				= entt::type_info<audio::SoundAsset>::id(),
		Sprite				= entt::type_info<render::SpriteAsset>::id(),
		Texture				= entt::type_info<render::TextureAsset>::id(),
	};

	struct AssetEntry
	{
		core::EAssetType	m_Type;
		str::Name			m_Guid;
		str::Path			m_Filepath;
	};
}