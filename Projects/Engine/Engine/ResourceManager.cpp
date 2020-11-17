#include "Engine/ResourceManager.h"

#include "Engine/Path.h"
#include "Engine/StringHelpers.h"

#include <filesystem>
#include <entt/core/type_info.hpp>

namespace
{
	constexpr char* s_AssetsDirectory = "Assets/";

	core::EResourceType ToResourceType(const char* string)
	{
		if (str::Equals(string, "physics_material"))
			return core::EResourceType::PhysicsMaterial;
		if (str::Equals(string, "sound"))
			return core::EResourceType::Sound;
		if (str::Equals(string, "texture"))
			return core::EResourceType::Texture;
		return core::EResourceType::Unknown;
	}
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
		if (entry.is_directory() && isSearchingSubdirectories)
		{
			LoadDirectory(entry.path().string().c_str(), true);
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

				const str::Name name = str::Name::Create(guidString);
				const EResourceType type = ToResourceType(typeString);
				m_ResourceMap.insert({ name, ResourceEntry{ name, filepath, type } });
			}
		}
	}

	for (auto&& [key, value] : m_ResourceMap)
	{
		switch (value.m_Type)
		{
		case EResourceType::PhysicsMaterial:
			LoadResource<physics::MaterialResource>(value.m_Name);
			break;
		case EResourceType::Sound:
			LoadResource<audio::SoundResource>(value.m_Name);
			break;
		case EResourceType::Texture:
			LoadResource<render::TextureResource>(value.m_Name);
			break;
		}
	}
}
