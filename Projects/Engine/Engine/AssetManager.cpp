#include "Engine/EnginePCH.h"
#include "Engine/AssetManager.h"

#include "Engine/AssetHelpers.h"
#include "Engine/JsonHelpers.h"
#include "Engine/StringHelpers.h"

#include <filesystem>

namespace
{
	constexpr char* s_AssetExtension = ".asset";
	constexpr char* s_AssetsDirectory = "Assets\\";

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
	LoadDirectory(s_AssetsDirectory, true);
}

void core::AssetManager::Destroy()
{
	m_PhysicsMaterialCache.clear();
	m_SoundCache.clear();
	m_TextureCache.clear();
}

void core::AssetManager::LoadDirectory(const char* directory, const bool isSearchingSubdirectories)
{
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		const std::filesystem::path& path = entry.path();
		if (entry.is_directory() && isSearchingSubdirectories)
		{
			LoadDirectory(path.string().c_str(), true);
		}
		else
		{
			const str::Path filepath = entry.path().string();
			if (filepath.HasFileExtension(s_AssetExtension))
			{
				LoadFile(filepath.ToChar());
			}
		}
	}
}

void core::AssetManager::LoadFile(const char* filepath)
{
	rapidjson::Document document;
	json::LoadDocument(filepath, document);

	const char* guidString = json::ParseString(document, "asset_guid", nullptr);
	const char* typeString = json::ParseString(document, "asset_type", nullptr);

	const str::Name guid = str::Name::Create(guidString);
	const core::EAssetType type = core::ToAssetType(typeString);
	m_AssetEntryMap.insert({ guid, AssetEntry{ type, guid, filepath } });
}
