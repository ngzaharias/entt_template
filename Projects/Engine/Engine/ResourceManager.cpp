#include "Engine/ResourceManager.h"

#include "Engine/Path.h"
#include "Engine/StringHelpers.h"

#include <filesystem>
#include <entt/core/type_info.hpp>

namespace
{
	constexpr char* s_AssetsDirectory = "Assets\\";
}

core::ResourceManager::ResourceManager(physics::PhysicsManager& physicsManager)
	: m_PhysicsManager(physicsManager)
{
}

core::ResourceManager::~ResourceManager()
{
}

void core::ResourceManager::Initialize()
{
	LoadDirectory(s_AssetsDirectory, true);
}

void core::ResourceManager::Destroy()
{
	m_PhysicsMaterialCache.clear();
	m_SoundCache.clear();
	m_TextureCache.clear();
}

void core::ResourceManager::LoadDirectory(const char* directory, const bool isSearchingSubdirectories)
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
			if (filepath.HasFileExtension(".asset"))
			{
				rapidjson::Document document;
				json::LoadDocument(filepath.ToChar(), document);

				const char* guidString = json::ParseString(document, "resource_guid", nullptr);
				const char* typeString = json::ParseString(document, "resource_type", nullptr);

				const str::Name guid = str::Name::Create(guidString);
				const EResourceType type = core::ToResourceType(typeString);
				m_ResourceEntries.insert({ guid, ResourceEntry{ guid, filepath, type } });
			}
		}
	}
}
