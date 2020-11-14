#pragma once

#include <Engine/JsonHelpers.h>
#include <Engine/PhysicsManager.h>
#include <Engine/Resource.h>
#include <Engine/Types.h>

#include <entt/resource/cache.hpp>
#include <entt/resource/handle.hpp>
#include <PhysX/PxMaterial.h>
#include <PhysX/PxPhysics.h>

namespace physics
{
	struct MaterialResource : public core::Resource
	{
		physx::PxMaterial* m_Material = nullptr;
	};

	class MaterialLoader : public entt::resource_loader<MaterialLoader, MaterialResource>
	{
	public:
		std::shared_ptr<MaterialResource> load(physx::PxPhysics& physics, const str::Path& filepath) const
		{
			rapidjson::Document document;
			json::LoadDocument(filepath.ToChar(), document);

			const float32 static_friction = json::ParseFloat(document, "static_friction", 0.f);
			const float32 dynamic_friction = json::ParseFloat(document, "dynamic_friction", 0.f);
			const float32 restituation = json::ParseFloat(document, "restituation", 0.f);

			MaterialResource* resource = new MaterialResource();
			resource->m_Filepath = filepath;
			resource->m_Material = physics.createMaterial(static_friction, dynamic_friction, restituation);
			return std::shared_ptr<MaterialResource>(resource);
		}
	};

	using MaterialCache = entt::resource_cache<MaterialResource>;
	using MaterialHandle = entt::resource_handle<MaterialResource>;
}