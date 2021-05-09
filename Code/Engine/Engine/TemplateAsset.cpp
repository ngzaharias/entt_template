#include "EnginePCH.h"
#include "Engine/TemplateAsset.h"

#include "Engine/AssetHelpers.h"
#include "Engine/AssetTypes.h"
#include "Engine/JsonHelpers.h"
#include "Engine/JsonTypes.h"

bool core::TemplateLoader::save(const core::TemplateAsset& asset, const core::AssetEntry& entry) const
{
	static const char* s_AssetType = core::ToAssetType(core::EAssetType::Template);

	json::Document document;
	json::Object asset_guid;
	json::Object asset_type;
	auto& allocator = document.GetAllocator();

	document.SetObject();
	asset_guid.SetString(entry.m_Guid.ToChar(), allocator);
	asset_type.SetString(s_AssetType, allocator);

	document.AddMember("asset_guid", asset_guid, allocator);
	document.AddMember("asset_type", asset_type, allocator);

	return json::SaveDocument(entry.m_Filepath, document);
}

core::AssetPtr<core::TemplateAsset> core::TemplateLoader::load(const core::AssetEntry& entry) const
{
	json::Document document;
	json::LoadDocument(entry.m_Filepath, document);

	core::TemplateAsset* Asset = new core::TemplateAsset();
	Asset->m_Guid = entry.m_Guid;
	Asset->m_Filepath = entry.m_Filepath;

	return core::AssetPtr<core::TemplateAsset>(Asset);
}