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
void core::ResourceManager::CreateResource<physics::MaterialResource>(const str::Path& folder)
{
	core::ResourceEntry entry = GenerateEntry(folder);

	physics::MaterialLoader loader;
	if (loader.save(entry))
	{
		m_ResourceEntries[entry.m_Guid] = entry;
	}
}

template<class TResource>
entt::resource_handle<TResource> core::ResourceManager::LoadResource(const str::Name& name)
{
	static_assert(false, "Resource Type doesn't exist!");
	return nullptr;
}

template<>
physics::MaterialHandle core::ResourceManager::LoadResource<physics::MaterialResource>(const str::Name& name)
{
	const core::ResourceEntry& resourceEntry = m_ResourceEntries[name];
	return m_PhysicsMaterialCache.load<physics::MaterialLoader>(name.ToHash(), resourceEntry, m_PhysicsManager);
}

template<>
audio::SoundHandle core::ResourceManager::LoadResource<audio::SoundResource>(const str::Name& name)
{
	const core::ResourceEntry& resourceEntry = m_ResourceEntries[name];
	return m_SoundCache.load<audio::SoundLoader>(name.ToHash(), resourceEntry);
}

template<>
render::TextureHandle core::ResourceManager::LoadResource<render::TextureResource>(const str::Name& name)
{
	const core::ResourceEntry& resourceEntry = m_ResourceEntries[name];
	return m_TextureCache.load<render::TextureLoader>(name.ToHash(), resourceEntry);
}
