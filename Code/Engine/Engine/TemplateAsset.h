#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetCache.h>
#include <Engine/AssetHandle.h>
#include <Engine/AssetLoader.h>
#include <Engine/AssetPtr.h>

namespace core
{
	struct AssetEntry;

	struct TemplateAsset : public core::Asset
	{
	};

	class TemplateLoader : public core::AssetLoader<TemplateLoader, TemplateAsset>
	{
	public:
		bool save(const TemplateAsset& asset, const core::AssetEntry& entry) const;
		core::AssetPtr<TemplateAsset> load(const core::AssetEntry& entry) const;
	};

	using TemplateCache = core::AssetCache<TemplateAsset>;
	using TemplateHandle = core::AssetHandle<TemplateAsset>;
}