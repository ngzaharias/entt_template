#include "Engine/EnginePCH.h"
#include "Engine/TextureAsset.h"

#include "Engine/AssetTypes.h"
#include "Engine/FileHelpers.h"
#include "Engine/JsonHelpers.h"

#include <SFML/System/FileInputStream.hpp>

bool render::TextureLoader::Import(const str::Path& inputPath, const str::Path& outputPath) const
{
	str::String AssetGuid = str::GenerateGUID();

	str::String binaryData;
	core::LoadFileAsBinary(inputPath, binaryData);
	rapidjson::SizeType binarySize = static_cast<rapidjson::SizeType>(binaryData.size());

	rapidjson::Document document;
	rapidjson::Value asset_guid;
	rapidjson::Value asset_type;
	rapidjson::Value source_file;
	rapidjson::Value binary_data;

	document.SetObject();
	asset_guid.SetString(AssetGuid.c_str(), document.GetAllocator());
	asset_type.SetString("texture", document.GetAllocator());
	source_file.SetString(inputPath.ToChar(), document.GetAllocator());
	binary_data.SetString(binaryData.c_str(), binarySize, document.GetAllocator());

	document.AddMember("asset_guid", asset_guid, document.GetAllocator());
	document.AddMember("asset_type", asset_type, document.GetAllocator());
	document.AddMember("source_file", source_file, document.GetAllocator());
	document.AddMember("binary_data", binary_data, document.GetAllocator());

	json::SaveDocument(outputPath.ToChar(), document);

	return true;
}

core::AssetPtr<render::TextureAsset> render::TextureLoader::load(const core::AssetEntry& entry) const
{
	rapidjson::Document document;
	json::LoadDocument(entry.m_Filepath.ToChar(), document);

	str::Path sourceFile = json::ParseString(document, "source_file", "");
	json::Binary binaryData = json::ParseBinary(document, "binary_data", json::Binary());

	render::TextureAsset* Asset = new render::TextureAsset();
	Asset->m_Guid = entry.m_Guid;
	Asset->m_Filepath = entry.m_Filepath;
	Asset->m_SourceFile = sourceFile;
	Asset->m_Texture.loadFromMemory(binaryData.m_Data, binaryData.m_Size);

	return core::AssetPtr<render::TextureAsset>(Asset);
}