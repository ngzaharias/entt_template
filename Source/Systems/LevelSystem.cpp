#include "LevelSystem.hpp"

#include <entt/entt.hpp>
#include <rapidjson/document.h>
#include <SFML/Graphics.hpp>

core::LevelSystem::LevelSystem()
{
}

core::LevelSystem::~LevelSystem()
{
}

void core::LevelSystem::Update(entt::registry& registry, const sf::Time& time)
{
}

bool core::LevelSystem::Load(entt::registry& registry)
{
	return true;
}

void core::LevelSystem::Unload(entt::registry& registry)
{
}
