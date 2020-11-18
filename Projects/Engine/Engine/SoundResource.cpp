#include "Engine/SoundResource.h"

#include "Engine/JsonHelpers.h"
#include "Engine/ResourceTypes.h"

std::shared_ptr<audio::SoundResource> audio::SoundLoader::load(const core::ResourceEntry& resourceEntry) const
{
	rapidjson::Document document;
	json::LoadDocument(resourceEntry.m_Filepath.ToChar(), document);

	str::Path sourceFile = json::ParseString(document, "source_file", nullptr);

	audio::SoundResource* resource = new audio::SoundResource();
	resource->m_Name = resourceEntry.m_Name;
	resource->m_SoundBuffer.loadFromFile(sourceFile.ToChar());
	resource->m_SourceFile = sourceFile;
	return std::shared_ptr<audio::SoundResource>(resource);
}