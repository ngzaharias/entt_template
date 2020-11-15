#pragma once

#include <entt/core/hashed_string.hpp>
#include <PhysX/PxPhysics.h>

template<>
physics::MaterialHandle core::ResourceManager::GetResource<physics::MaterialResource>(const str::Guid& guid)
{
	return m_PhysicsMaterialCache.handle(guid.ToHash());
}

template<>
physics::MaterialHandle core::ResourceManager::LoadResource<physics::MaterialResource>(const str::Guid& guid)
{
	physx::PxPhysics& physics = m_PhysicsManager.GetPhysics();
	return m_PhysicsMaterialCache.load<physics::MaterialLoader>(guid.ToHash(), physics, m_ResourceMap[guid].m_Filepath);
}

template<>
audio::SoundHandle core::ResourceManager::GetResource<audio::SoundResource>(const str::Guid& guid)
{
	return m_SoundCache.handle(guid.ToHash());
}

template<>
audio::SoundHandle core::ResourceManager::LoadResource<audio::SoundResource>(const str::Guid& guid)
{
	return m_SoundCache.load<audio::SoundLoader>(guid.ToHash(), m_ResourceMap[guid].m_Filepath);
}

template<>
render::TextureHandle core::ResourceManager::GetResource<render::TextureResource>(const str::Guid& guid)
{
	return m_TextureCache.handle(guid.ToHash());
}

template<>
render::TextureHandle core::ResourceManager::LoadResource<render::TextureResource>(const str::Guid& guid)
{
	return m_TextureCache.load<render::TextureLoader>(guid.ToHash(), m_ResourceMap[guid].m_Filepath);
}
