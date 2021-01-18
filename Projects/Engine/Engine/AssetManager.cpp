#include "Engine/EnginePCH.h"
#include "Engine/AssetManager.h"

#include "Engine/AssetHelpers.h"
#include "Engine/JsonHelpers.h"
#include "Engine/StringHelpers.h"

#include <filesystem>

namespace
{
	constexpr wchar_t* s_AssetExtension = L".asset";
	constexpr wchar_t* s_AssetsDirectory = L"Assets\\";

	core::AssetManager* s_Instance = nullptr;
}

core::AssetManager& core::AssetManager::Instance()
{
	return *s_Instance;
}

core::AssetManager::AssetManager(physics::PhysicsManager& physicsManager)
	: m_PhysicsManager(physicsManager)
{
	assert(!s_Instance);
	s_Instance = this;
}

core::AssetManager::~AssetManager()
{
}

void core::AssetManager::Initialize()
{
	LoadFilepath(s_AssetsDirectory, true);
}

void core::AssetManager::Destroy()
{
	m_PhysicsMaterialCache.clear();
	m_SoundCache.clear();
	m_TextureCache.clear();
}

void core::AssetManager::CreateAsset(const render::FlipbookAsset& asset, const str::Path& folderPath)
{
	core::AssetEntry entry = GenerateEntry(core::EAssetType::Flipbook, folderPath);

	render::FlipbookLoader loader;
	if (loader.save(asset, entry))
	{
		m_AssetEntryMap[entry.m_Guid] = entry;
	}
}

void core::AssetManager::CreateAsset(const physics::MaterialAsset& asset, const str::Path& folderPath)
{
	core::AssetEntry entry = GenerateEntry(core::EAssetType::PhysicsMaterial, folderPath);

	physics::MaterialLoader loader;
	if (loader.save(asset, entry))
	{
		m_AssetEntryMap[entry.m_Guid] = entry;
	}
}

void core::AssetManager::CreateAsset(const render::SpriteAsset& asset, const str::Path& folderPath)
{
	core::AssetEntry entry = GenerateEntry(core::EAssetType::Sprite, folderPath);

	render::SpriteLoader loader;
	if (loader.save(asset, entry))
	{
		m_AssetEntryMap[entry.m_Guid] = entry;
	}
}

void core::AssetManager::SaveAsset(const render::FlipbookAsset& asset)
{
	const core::AssetEntry& entry = m_AssetEntryMap[asset.m_Guid];

	render::FlipbookLoader loader;
	loader.save(asset, entry);
}

void core::AssetManager::SaveAsset(const render::SpriteAsset& asset)
{
	const core::AssetEntry& entry = m_AssetEntryMap[asset.m_Guid];

	render::SpriteLoader loader;
	loader.save(asset, entry);
}

void core::AssetManager::LoadFilepath(const str::Path& filepath, const bool isSearchingSubdirectories)
{
	auto type = filepath.c_str();

	if (filepath.has_filename() && filepath.has_extension())
	{
		if (filepath.extension() == s_AssetExtension)
		{
			rapidjson::Document document;
			json::LoadDocument(filepath, document);

			const char* guidString = json::ParseString(document, "asset_guid", nullptr);
			const char* typeString = json::ParseString(document, "asset_type", nullptr);

			const str::Name guid = str::Name::Create(guidString);
			const core::EAssetType type = core::ToAssetType(typeString);
			m_AssetEntryMap.insert({ guid, AssetEntry{ type, guid, filepath } });
		}
	}
	else
	{
		for (const auto& entry : std::filesystem::directory_iterator(filepath))
		{
			const str::Path& subpath = entry.path();
			LoadFilepath(subpath, isSearchingSubdirectories);
		}
	}
}
