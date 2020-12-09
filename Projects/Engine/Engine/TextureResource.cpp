#include "Engine/EnginePCH.h"
#include "Engine/TextureResource.h"

#include "Engine/JsonHelpers.h"
#include "Engine/ResourceTypes.h"

#include <SFML/System/FileInputStream.hpp>

std::shared_ptr<render::TextureResource> render::TextureLoader::load(const core::ResourceEntry& resourceEntry) const
{
	rapidjson::Document document;
	json::LoadDocument(resourceEntry.m_Filepath.ToChar(), document);

	str::Path sourceFile = json::ParseString(document, "source_file", "");
	json::Binary binaryData = json::ParseBinary(document, "binary_data", json::Binary());

	render::TextureResource* resource = new render::TextureResource();
	resource->m_Guid = resourceEntry.m_Guid;
	resource->m_SourceFile = sourceFile;
	resource->m_Texture.loadFromMemory(binaryData.m_Data, binaryData.m_Size);

	return std::shared_ptr<render::TextureResource>(resource);
}