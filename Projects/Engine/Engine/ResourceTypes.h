#pragma once

#include <Engine/ResourceTypes.h>

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

	template<class TResource>
	core::EResourceType ToResourceType()
	{
		constexpr entt::id_type typeId = entt::type_info<TResource>::id();
		return static_cast<EResourceType>(typeId);
	}
	core::EResourceType ToResourceType(const char* string);
	const char* ToResourceType(core::EResourceType resourceType);

	struct ResourceEntry
	{
		str::Name m_Guid;
		str::Path m_Filepath;
		EResourceType m_Type;
	};

	template<typename TResource>
	struct ResourceHandle
	{
		str::Name m_Guid;
		EResourceType m_Type = ToResourceType<TResource>();
	};
}