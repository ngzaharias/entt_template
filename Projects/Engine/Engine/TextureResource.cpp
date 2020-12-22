#include "Engine/EnginePCH.h"
#include "Engine/TextureResource.h"

#include "Engine/FileHelpers.h"
#include "Engine/JsonHelpers.h"
#include "Engine/ResourceTypes.h"

#include <SFML/System/FileInputStream.hpp>

bool render::TextureLoader::Import(const str::Path& inputPath, const str::Path& outputPath) const
{
	str::String resourceGuid = str::GenerateGUID();

	str::String binaryData;
	core::LoadFileAsBinary(inputPath, binaryData);
	rapidjson::SizeType binarySize = static_cast<rapidjson::SizeType>(binaryData.size());

	rapidjson::Document document;
	rapidjson::Value resource_guid;
	rapidjson::Value resource_type;
	rapidjson::Value source_file;
	rapidjson::Value binary_data;

	document.SetObject();
	resource_guid.SetString(resourceGuid.c_str(), document.GetAllocator());
	resource_type.SetString("texture", document.GetAllocator());
	source_file.SetString(inputPath.ToChar(), document.GetAllocator());
	binary_data.SetString(binaryData.c_str(), binarySize, document.GetAllocator());

	document.AddMember("resource_guid", resource_guid, document.GetAllocator());
	document.AddMember("resource_type", resource_type, document.GetAllocator());
	document.AddMember("source_file", source_file, document.GetAllocator());
	document.AddMember("binary_data", binary_data, document.GetAllocator());

	json::SaveDocument(outputPath.ToChar(), document);

	return true;
}

std::shared_ptr<render::TextureResource> render::TextureLoader::load(const core::ResourceEntry& entry) const
{
	rapidjson::Document document;
	json::LoadDocument(entry.m_Filepath.ToChar(), document);

	str::Path sourceFile = json::ParseString(document, "source_file", "");
	json::Binary binaryData = json::ParseBinary(document, "binary_data", json::Binary());

	render::TextureResource* resource = new render::TextureResource();
	resource->m_Guid = entry.m_Guid;
	resource->m_Filepath = entry.m_Filepath;
	resource->m_SourceFile = sourceFile;
	resource->m_Texture.loadFromMemory(binaryData.m_Data, binaryData.m_Size);

	return std::shared_ptr<render::TextureResource>(resource);
}