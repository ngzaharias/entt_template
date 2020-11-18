#include "Engine/PhysicsMaterialResource.h"

#include "Engine/JsonHelpers.h"
#include "Engine/PhysicsManager.h"
#include "Engine/ResourceTypes.h"

#include <PhysX/PxMaterial.h>
#include <PhysX/PxPhysics.h>

std::shared_ptr<physics::MaterialResource> physics::MaterialLoader::load(const core::ResourceEntry& resourceEntry, const physics::PhysicsManager& physicsManager) const
{
	physx::PxPhysics& physics = physicsManager.GetPhysics();

	rapidjson::Document document;
	json::LoadDocument(resourceEntry.m_Filepath.ToChar(), document);

	const float static_friction = json::ParseFloat(document, "static_friction", 0.f);
	const float dynamic_friction = json::ParseFloat(document, "dynamic_friction", 0.f);
	const float restituation = json::ParseFloat(document, "restituation", 0.f);

	physics::MaterialResource* resource = new physics::MaterialResource();
	resource->m_Name = resourceEntry.m_Name;
	resource->m_Material = physics.createMaterial(static_friction, dynamic_friction, restituation);
	return std::shared_ptr<physics::MaterialResource>(resource);
}