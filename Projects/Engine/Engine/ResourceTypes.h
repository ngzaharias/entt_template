#pragma once

#include <Engine/Name.h>
#include <Engine/Path.h>

#include <entt/fwd.hpp>
#include <entt/core/type_info.hpp>

namespace audio
{
	struct SoundResource;
}

namespace physics
{
	struct MaterialResource;
}

namespace render
{
	struct TextureResource;
}

namespace core
{
	enum class EResourceType : entt::id_type
	{
		Unknown,

		PhysicsMaterial		= entt::type_info<physics::MaterialResource>::id(),
		Sound				= entt::type_info<audio::SoundResource>::id(),
		Texture				= entt::type_info<render::TextureResource>::id(),
	};

	core::EResourceType ToResourceType(const char* string);
	const char* ToResourceType(core::EResourceType resourceType);

	struct ResourceEntry
	{
		str::Name m_Guid;
		str::Path m_Filepath;
		EResourceType m_Type;
	};
}