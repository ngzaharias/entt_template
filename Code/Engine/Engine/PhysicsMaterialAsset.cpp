#include "EnginePCH.h"
#include "Engine/PhysicsMaterialAsset.h"

#include "Engine/AssetHelpers.h"
#include "Engine/AssetTypes.h"
#include "Engine/JsonHelpers.h"
#include "Engine/PhysicsManager.h"

#include <PhysX/PxMaterial.h>
#include <PhysX/PxPhysics.h>

bool physics::MaterialLoader::save(const MaterialAsset& asset, const core::AssetEntry& entry) const
{
	static const char* s_AssetType = core::ToAssetType(core::EAssetType::PhysicsMaterial);

	rapidjson::Document document;
	rapidjson::Value asset_guid;
	rapidjson::Value asset_type;
	rapidjson::Value static_friction;
	rapidjson::Value dynamic_friction;
	rapidjson::Value restituation;

	document.SetObject();
	asset_guid.SetString(entry.m_Guid.ToChar(), document.GetAllocator());
	asset_type.SetString(s_AssetType, document.GetAllocator());
	static_friction.SetFloat(0.f);
	dynamic_friction.SetFloat(0.f);
	restituation.SetFloat(0.f);

	document.AddMember("asset_guid", asset_guid, document.GetAllocator());
	document.AddMember("asset_type", asset_type, document.GetAllocator());
	document.AddMember("static_friction", static_friction, document.GetAllocator());
	document.AddMember("dynamic_friction", dynamic_friction, document.GetAllocator());
	document.AddMember("restituation", restituation, document.GetAllocator());

	return json::SaveDocument(entry.m_Filepath, document);
}

core::AssetPtr<physics::MaterialAsset> physics::MaterialLoader::load(const core::AssetEntry& entry, const physics::PhysicsManager& physicsManager) const
{
	physx::PxPhysics& physics = physicsManager.GetPhysics();

	rapidjson::Document document;
	json::LoadDocument(entry.m_Filepath, document);

	const float static_friction = json::ParseFloat(document, "static_friction", 0.f);
	const float dynamic_friction = json::ParseFloat(document, "dynamic_friction", 0.f);
	const float restituation = json::ParseFloat(document, "restituation", 0.f);

	physics::MaterialAsset* asset = new physics::MaterialAsset();
	asset->m_Guid = entry.m_Guid;
	asset->m_Filepath = entry.m_Filepath;
	asset->m_Material = physics.createMaterial(static_friction, dynamic_friction, restituation);
	return core::AssetPtr<physics::MaterialAsset>(asset);
}