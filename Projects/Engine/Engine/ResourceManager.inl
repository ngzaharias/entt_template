#pragma once

#include <PhysX/PxPhysics.h>

template<class TResource>
entt::resource_handle<TResource> core::ResourceManager::LoadResource(const str::Name& name)
{
	static_assert(false, "Resource Type doesn't exist!");
	return nullptr;
}

template<>
physics::MaterialHandle core::ResourceManager::LoadResource<physics::MaterialResource>(const str::Name& name)
{
	const core::ResourceEntry& resourceEntry = m_ResourceMap[name];
	return m_PhysicsMaterialCache.load<physics::MaterialLoader>(name.ToHash(), resourceEntry, m_PhysicsManager);
}

template<>
audio::SoundHandle core::ResourceManager::LoadResource<audio::SoundResource>(const str::Name& name)
{
	const core::ResourceEntry& resourceEntry = m_ResourceMap[name];
	return m_SoundCache.load<audio::SoundLoader>(name.ToHash(), resourceEntry);
}

template<>
render::TextureHandle core::ResourceManager::LoadResource<render::TextureResource>(const str::Name& name)
{
	const core::ResourceEntry& resourceEntry = m_ResourceMap[name];
	return m_TextureCache.load<render::TextureLoader>(name.ToHash(), resourceEntry);
}
