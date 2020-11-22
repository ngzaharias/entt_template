#pragma once

#include <Engine/Resource.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>

namespace core
{
	struct ResourceEntry;
}

namespace physx
{
	class PxMaterial;
}

namespace physics
{
	class PhysicsManager;

	struct MaterialResource : public core::Resource
	{
		physx::PxMaterial* m_Material = nullptr;
	};

	class MaterialLoader : public entt::resource_loader<MaterialLoader, MaterialResource>
	{
	public:
		bool save(const core::ResourceEntry& entry) const;
		std::shared_ptr<MaterialResource> load(const core::ResourceEntry& entry, const physics::PhysicsManager& physicsManager) const;
	};

	using MaterialCache = entt::resource_cache<MaterialResource>;
	using MaterialHandle = entt::resource_handle<MaterialResource>;
}