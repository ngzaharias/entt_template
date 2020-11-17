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

	struct ResourceEntry
	{
		str::Name m_Name;
		str::Path m_Filepath;
		EResourceType m_Type;
	};
}