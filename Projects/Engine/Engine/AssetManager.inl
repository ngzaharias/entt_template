#pragma once

#include "Engine/StringHelpers.h"

#include <filesystem>
#include <PhysX/PxPhysics.h>

namespace
{
	core::AssetEntry GenerateEntry(const core::EAssetType& type, const str::Path& folder)
	{
		core::AssetEntry entry;
		entry.m_Guid = str::Name::Create(str::GenerateGUID());
		entry.m_Filepath = folder;
		entry.m_Type = type;
		return entry;
	}
}

template<class Type>
void core::AssetManager::CreateAsset(const str::Path& filepath)
{
	static_assert(false, "Asset Type doesn't exist!");
	return nullptr;
}

template<>
void core::AssetManager::CreateAsset<physics::MaterialAsset>(const str::Path& folderPath)
{
	core::AssetEntry entry = GenerateEntry(core::EAssetType::PhysicsMaterial, folderPath);

	physics::MaterialLoader loader;
	if (loader.save(entry))
	{
		m_AssetEntryMap[entry.m_Guid] = entry;
	}
}

template<class Type>
void core::AssetManager::ImportAsset(const str::Path& inputPath, const str::Path& outputPath)
{
	static_assert(false, "Asset Type doesn't exist!");
	return nullptr;
}

template<>
void core::AssetManager::ImportAsset<audio::SoundAsset>(const str::Path& inputPath, const str::Path& outputPath)
{
	audio::SoundLoader loader;
	loader.Import(inputPath, outputPath);
}

template<>
void core::AssetManager::ImportAsset<render::TextureAsset>(const str::Path& inputPath, const str::Path& outputPath)
{
	render::TextureLoader loader;
	loader.Import(inputPath, outputPath);
}

template<class Type>
core::AssetHandle<Type> core::AssetManager::LoadAsset(const str::Name& guid)
{
	static_assert(false, "Asset Type doesn't exist!");
	return nullptr;
}

template<>
core::EntityTemplateHandle core::AssetManager::LoadAsset<core::EntityTemplateAsset>(const str::Name& guid)
{
	const auto find = m_AssetEntryMap.find(guid);
	if (find == m_AssetEntryMap.end())
		return { };

	const core::AssetEntry& assetEntry = find->second;
	return m_EntityTemplateCache.load<core::EntityTemplateLoader>(guid.ToHash(), assetEntry);
}

template<>
physics::MaterialHandle core::AssetManager::LoadAsset<physics::MaterialAsset>(const str::Name& guid)
{
	const auto find = m_AssetEntryMap.find(guid);
	if (find == m_AssetEntryMap.end())
		return { };

	const core::AssetEntry& assetEntry = find->second;
	return m_PhysicsMaterialCache.load<physics::MaterialLoader>(guid.ToHash(), assetEntry, m_PhysicsManager);
}

template<>
audio::SoundHandle core::AssetManager::LoadAsset<audio::SoundAsset>(const str::Name& guid)
{
	const auto find = m_AssetEntryMap.find(guid);
	if (find == m_AssetEntryMap.end())
		return { };

	const core::AssetEntry& assetEntry = find->second;
	return m_SoundCache.load<audio::SoundLoader>(guid.ToHash(), assetEntry);
}

template<>
render::TextureHandle core::AssetManager::LoadAsset<render::TextureAsset>(const str::Name& guid)
{
	const auto find = m_AssetEntryMap.find(guid);
	if (find == m_AssetEntryMap.end())
		return { };

	const core::AssetEntry& assetEntry = find->second;
	return m_TextureCache.load<render::TextureLoader>(guid.ToHash(), assetEntry);
}
