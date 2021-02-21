#pragma once

#include "Engine/StringHelpers.h"

#include <filesystem>
#include <PhysX/PxPhysics.h>

namespace
{
	core::AssetEntry GenerateEntry(const core::EAssetType& type, const str::Path& folder)
	{
		core::AssetEntry entry;
		entry.m_Guid = NAME(str::GenerateGUID());
		entry.m_Filepath = folder;
		entry.m_Type = type;
		return entry;
	}
}

template<class Type>
str::Name core::AssetManager::CreateAsset(const Type& asset, const str::Path& filepath)
{
	static_assert(false, "Can't Create an asset of Type!");
	return str::strNullGuid;
}

template<class Type>
void core::AssetManager::SaveAsset(const Type& asset)
{
	static_assert(false, "Can't Save an asset of Type!");
	return nullptr;
}

template<class Type>
void core::AssetManager::ImportAsset(const str::Path& inputPath, const str::Path& outputPath)
{
	static_assert(false, "Can't Import an asset of Type!");
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
	static_assert(false, "Can't Load an asset of Type!");
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
render::FlipbookHandle core::AssetManager::LoadAsset<render::FlipbookAsset>(const str::Name& guid)
{
	const auto find = m_AssetEntryMap.find(guid);
	if (find == m_AssetEntryMap.end())
		return { };

	const core::AssetEntry& assetEntry = find->second;
	return m_FlipbookCache.load<render::FlipbookLoader>(guid.ToHash(), assetEntry);
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
render::SpriteHandle core::AssetManager::LoadAsset<render::SpriteAsset>(const str::Name& guid)
{
	const auto find = m_AssetEntryMap.find(guid);
	if (find == m_AssetEntryMap.end())
		return { };

	const core::AssetEntry& assetEntry = find->second;
	return m_SpriteCache.load<render::SpriteLoader>(guid.ToHash(), assetEntry);
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
