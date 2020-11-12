#include "ResourceManager.h"

#include "Strings/Path.h"

#include <filesystem>
#include <iostream>

namespace
{
	constexpr char* s_SoundsDirectory = "Assets/Sounds/";
	constexpr char* s_TexturesDirectory = "Assets/Textures/";
}

core::ResourceManager::ResourceManager()
{
}

core::ResourceManager::~ResourceManager()
{
}

void core::ResourceManager::Initialize()
{
	for (const auto& entry : std::filesystem::directory_iterator(s_SoundsDirectory))
	{
		const str::Path filepath = entry.path().string();
		LoadResource<core::SoundResource>(filepath);
	}

	for (const auto& entry : std::filesystem::directory_iterator(s_TexturesDirectory))
	{
		const str::Path filepath = entry.path().string();
		LoadResource<core::TextureResource>(filepath);
	}
}

void core::ResourceManager::Destroy()
{
	m_SoundCache.clear();
	m_TextureCache.clear();
}