#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetCache.h>
#include <Engine/AssetHandle.h>
#include <Engine/AssetLoader.h>
#include <Engine/AssetPtr.h>

namespace core
{
	struct AssetEntry;

	struct EntityTemplateAsset : public core::Asset
	{
	};

	class EntityTemplateLoader : public core::AssetLoader<EntityTemplateLoader, EntityTemplateAsset>
	{
	public:
		core::AssetPtr<EntityTemplateAsset> load(const core::AssetEntry& entry) const;
	};

	using EntityTemplateCache = core::AssetCache<EntityTemplateAsset>;
	using EntityTemplateHandle = core::AssetHandle<EntityTemplateAsset>;
}