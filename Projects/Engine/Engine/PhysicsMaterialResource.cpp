#include "Engine/PhysicsMaterialResource.h"

#include "Engine/JsonHelpers.h"
#include "Engine/PhysicsManager.h"
#include "Engine/Types.h"

#include <PhysX/PxMaterial.h>
#include <PhysX/PxPhysics.h>

std::shared_ptr<physics::MaterialResource> physics::MaterialLoader::load(const str::Path& filepath) const
{
	physx::PxPhysics& physics = physics::PhysicsManager::Instance().GetPhysics();

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
