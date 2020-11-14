#pragma once

#include <entt/core/hashed_string.hpp>
#include <PhysX/PxPhysics.h>

template<>
physics::MaterialHandle core::ResourceManager::GetResource<physics::MaterialResource>(const str::Path& filepath)
{
	return m_PhysicsMaterialCache.handle(entt::hashed_string{ filepath.ToChar() });
}

template<>
physics::MaterialHandle core::ResourceManager::LoadResource<physics::MaterialResource>(const str::Path& filepath)
{
	physx::PxPhysics& physics = m_PhysicsManager.GetPhysics();
	return m_PhysicsMaterialCache.load<physics::MaterialLoader>(entt::hashed_string{ filepath.ToChar() }, physics, filepath);
}

template<>
audio::SoundHandle core::ResourceManager::GetResource<audio::SoundResource>(const str::Path& filepath)
{
	return m_SoundCache.handle(entt::hashed_string{ filepath.ToChar() });
}

template<>
audio::SoundHandle core::ResourceManager::LoadResource<audio::SoundResource>(const str::Path& filepath)
{
	return m_SoundCache.load<audio::SoundLoader>(entt::hashed_string{ filepath.ToChar() }, filepath);
}

template<>
render::TextureHandle core::ResourceManager::GetResource<render::TextureResource>(const str::Path& filepath)
{
	return m_TextureCache.handle(entt::hashed_string{ filepath.ToChar() });
}

template<>
render::TextureHandle core::ResourceManager::LoadResource<render::TextureResource>(const str::Path& filepath)
{
	return m_TextureCache.load<render::TextureLoader>(entt::hashed_string{ filepath.ToChar() }, filepath);
}
