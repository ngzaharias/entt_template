#include "Engine/TextureResource.h"

#include "Engine/JsonHelpers.h"
#include "Engine/ResourceTypes.h"

std::shared_ptr<render::TextureResource> render::TextureLoader::load(const core::ResourceEntry& resourceEntry) const
{
	rapidjson::Document document;
	json::LoadDocument(resourceEntry.m_Filepath.ToChar(), document);

	str::Path sourceFile = json::ParseString(document, "source_file", nullptr);

	render::TextureResource* resource = new render::TextureResource();
	resource->m_Filepath = resourceEntry.m_Filepath;
	resource->m_SourceFile = sourceFile;
	resource->m_Texture.loadFromFile(sourceFile.ToChar());
	return std::shared_ptr<render::TextureResource>(resource);
}