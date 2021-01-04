#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetCache.h>
#include <Engine/AssetHandle.h>
#include <Engine/AssetLoader.h>
#include <Engine/AssetPtr.h>

namespace core
{
	struct AssetEntry;
}

namespace physx
{
	class PxMaterial;
}

namespace physics
{
	class PhysicsManager;

	struct MaterialAsset : public core::Asset
	{
		physx::PxMaterial* m_Material = nullptr;
	};

	class MaterialLoader : public core::AssetLoader<MaterialLoader, MaterialAsset>
	{
	public:
		bool save(const core::AssetEntry& entry) const;
		core::AssetPtr<MaterialAsset> load(const core::AssetEntry& entry, const physics::PhysicsManager& physicsManager) const;
	};

	using MaterialCache = core::AssetCache<MaterialAsset>;
	using MaterialHandle = core::AssetHandle<MaterialAsset>;
}