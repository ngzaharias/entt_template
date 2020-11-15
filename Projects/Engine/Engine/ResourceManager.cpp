#include "Engine/ResourceManager.h"

#include "Engine/Guid.h"
#include "Engine/Path.h"
#include "Engine/StringHelpers.h"

#include <filesystem>
#include <entt/core/type_info.hpp>

namespace
{
	constexpr char* s_AssetsDirectory = "Assets/";

	const entt::id_type ID_PhysicsMaterial = entt::type_info<physics::MaterialResource>::id();
	const entt::id_type ID_Sound = entt::type_info<audio::SoundResource>::id();
	const entt::id_type ID_Texture = entt::type_info<render::TextureResource>::id();
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
	LoadResources(s_AssetsDirectory, true);
}

void core::ResourceManager::Destroy()
{
	m_PhysicsMaterialCache.clear();
	m_SoundCache.clear();
	m_TextureCache.clear();
}

void core::ResourceManager::LoadResources(const char* directory, const bool isSearchingSubdirectories)
{
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (entry.is_directory() && isSearchingSubdirectories)
		{
			LoadResources(entry.path().string().c_str(), true);
		}
		else
		{
			const str::Path filepath = entry.path().string();
			if (filepath.HasFileExtension(".asset"))
			{
				rapidjson::Document document;
				json::LoadDocument(filepath.ToChar(), document);

				const char* resourceType = json::ParseString(document, "resource_type", nullptr);
				const char* resourceGUID = json::ParseString(document, "resource_guid", nullptr);

				entt::id_type typeId;
				if (str::Equals(resourceType, "physics_material"))
				{
					typeId = ID_PhysicsMaterial;
				}
				else if (str::Equals(resourceType, "sound"))
				{
					typeId = ID_Sound;
				}
				else if (str::Equals(resourceType, "texture"))
				{
					typeId = ID_Texture;
				}

				m_ResourceMap.insert({ resourceGUID, { resourceGUID, filepath, typeId } });
			}
		}
	}

	for (const auto& entry : m_ResourceMap)
	{
		switch (entry.second.m_TypeId)
		{
		case ID_PhysicsMaterial:
			LoadResource<physics::MaterialResource>(entry.second.m_Guid);
			break;
		case ID_Sound:
			LoadResource<audio::SoundResource>(entry.second.m_Guid);
			break;
		case ID_Texture:
			LoadResource<render::TextureResource>(entry.second.m_Guid);
			break;
		}
	}
}
