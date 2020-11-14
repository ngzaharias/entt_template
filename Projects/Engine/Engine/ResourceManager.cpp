#include "Engine/ResourceManager.h"

#include "Engine/Path.h"

#include <filesystem>

namespace
{
	constexpr char* s_PhysicsDirectory = "Assets/Physics/";
	constexpr char* s_SoundsDirectory = "Assets/Sounds/";
	constexpr char* s_TexturesDirectory = "Assets/Textures/";
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
	for (const auto& entry : std::filesystem::directory_iterator(s_PhysicsDirectory))
	{
		const str::Path filepath = entry.path().string();
		LoadResource<physics::MaterialResource>(filepath);
	}

	for (const auto& entry : std::filesystem::directory_iterator(s_SoundsDirectory))
	{
		const str::Path filepath = entry.path().string();
		LoadResource<audio::SoundResource>(filepath);
	}

	for (const auto& entry : std::filesystem::directory_iterator(s_TexturesDirectory))
	{
		const str::Path filepath = entry.path().string();
		LoadResource<render::TextureResource>(filepath);
	}
}

void core::ResourceManager::Destroy()
{
	m_PhysicsMaterialCache.clear();
	m_SoundCache.clear();
	m_TextureCache.clear();
}