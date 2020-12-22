#pragma once

#include "Engine/StringHelpers.h"

#include <filesystem>
#include <PhysX/PxPhysics.h>

namespace
{
	core::ResourceEntry GenerateEntry(const str::Path& folder)
	{
		std::filesystem::path filepath = folder.ToChar();

		core::ResourceEntry entry;
		entry.m_Guid = str::Name::Create(str::GenerateGUID());
		entry.m_Filepath = folder;
		entry.m_Type = core::EResourceType::PhysicsMaterial;
		return entry;
	}
}

template<class TResource>
void core::ResourceManager::CreateResource(const str::Path& filepath)
{
	static_assert(false, "Resource Type doesn't exist!");
	return nullptr;
}

template<>
void core::ResourceManager::CreateResource<physics::MaterialResource>(const str::Path& folderPath)
{
	core::ResourceEntry entry = GenerateEntry(folderPath);

	physics::MaterialLoader loader;
	if (loader.save(entry))
	{
		m_Entries[entry.m_Guid] = entry;
	}
}

template<class TResource>
void core::ResourceManager::ImportResource(const str::Path& inputPath, const str::Path& outputPath)
{
	static_assert(false, "Resource Type doesn't exist!");
	return nullptr;
}

template<>
void core::ResourceManager::ImportResource<audio::SoundResource>(const str::Path& inputPath, const str::Path& outputPath)
{
	audio::SoundLoader loader;
	loader.Import(inputPath, outputPath);
}

template<>
void core::ResourceManager::ImportResource<render::TextureResource>(const str::Path& inputPath, const str::Path& outputPath)
{
	render::TextureLoader loader;
	loader.Import(inputPath, outputPath);
}

template<class TResource>
entt::resource_handle<TResource> core::ResourceManager::LoadResource(const str::Name& guid)
{
	static_assert(false, "Resource Type doesn't exist!");
	return nullptr;
}

template<>
physics::MaterialPtr core::ResourceManager::LoadResource<physics::MaterialResource>(const str::Name& guid)
{
	const core::ResourceEntry& resourceEntry = m_Entries[guid];
	return m_PhysicsMaterialCache.load<physics::MaterialLoader>(guid.ToHash(), resourceEntry, m_PhysicsManager);
}

template<>
audio::SoundPtr core::ResourceManager::LoadResource<audio::SoundResource>(const str::Name& guid)
{
	const core::ResourceEntry& resourceEntry = m_Entries[guid];
	return m_SoundCache.load<audio::SoundLoader>(guid.ToHash(), resourceEntry);
}

template<>
render::TexturePtr core::ResourceManager::LoadResource<render::TextureResource>(const str::Name& guid)
{
	const core::ResourceEntry& resourceEntry = m_Entries[guid];
	return m_TextureCache.load<render::TextureLoader>(guid.ToHash(), resourceEntry);
}
