#include "Engine/PhysicsMaterialResource.h"

#include "Engine/JsonHelpers.h"
#include "Engine/PhysicsManager.h"
#include "Engine/ResourceTypes.h"

#include <PhysX/PxMaterial.h>
#include <PhysX/PxPhysics.h>

bool physics::MaterialLoader::save(const core::ResourceEntry& entry) const
{
	static const char* s_ResourceType = core::ToResourceType(core::EResourceType::PhysicsMaterial);

	rapidjson::Document document;
	rapidjson::Value resource_guid;
	rapidjson::Value resource_type;
	rapidjson::Value static_friction;
	rapidjson::Value dynamic_friction;
	rapidjson::Value restituation;

	document.SetObject();
	resource_guid.SetString(entry.m_Guid.ToChar(), document.GetAllocator());
	resource_type.SetString(s_ResourceType, document.GetAllocator());
	static_friction.SetFloat(0.f);
	dynamic_friction.SetFloat(0.f);
	restituation.SetFloat(0.f);

	document.AddMember("resource_guid", resource_guid, document.GetAllocator());
	document.AddMember("resource_type", resource_type, document.GetAllocator());
	document.AddMember("static_friction", static_friction, document.GetAllocator());
	document.AddMember("dynamic_friction", dynamic_friction, document.GetAllocator());
	document.AddMember("restituation", restituation, document.GetAllocator());

	return json::SaveDocument(entry.m_Filepath.ToChar(), document);
}

std::shared_ptr<physics::MaterialResource> physics::MaterialLoader::load(const core::ResourceEntry& entry, const physics::PhysicsManager& physicsManager) const
{
	physx::PxPhysics& physics = physicsManager.GetPhysics();

	rapidjson::Document document;
	json::LoadDocument(entry.m_Filepath.ToChar(), document);

	const float static_friction = json::ParseFloat(document, "static_friction", 0.f);
	const float dynamic_friction = json::ParseFloat(document, "dynamic_friction", 0.f);
	const float restituation = json::ParseFloat(document, "restituation", 0.f);

	physics::MaterialResource* resource = new physics::MaterialResource();
	resource->m_Guid = entry.m_Guid;
	resource->m_Material = physics.createMaterial(static_friction, dynamic_friction, restituation);
	return std::shared_ptr<physics::MaterialResource>(resource);
}