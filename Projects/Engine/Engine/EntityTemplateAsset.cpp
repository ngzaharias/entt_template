#include "Engine/EnginePCH.h"
#include "Engine/EntityTemplateAsset.h"

#include "Engine/AssetTypes.h"
#include "Engine/JsonHelpers.h"

core::AssetPtr<core::EntityTemplateAsset> core::EntityTemplateLoader::load(const core::AssetEntry& entry) const
{
	rapidjson::Document document;
	json::LoadDocument(entry.m_Filepath, document);

	str::Path sourceFile = json::ParseString(document, "source_file", "");
	json::Binary binaryData = json::ParseBinary(document, "binary_data", json::Binary());

	core::EntityTemplateAsset* Asset = new core::EntityTemplateAsset();
	Asset->m_Guid = entry.m_Guid;
	Asset->m_Filepath = entry.m_Filepath;

	return core::AssetPtr<core::EntityTemplateAsset>(Asset);
}