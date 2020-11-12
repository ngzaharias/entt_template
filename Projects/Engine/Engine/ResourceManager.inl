#pragma once

template<>
core::SoundHandle core::ResourceManager::GetResource<core::SoundResource>(const str::Path& filepath)
{
	return m_SoundCache.handle(entt::hashed_string{ filepath.ToChar() });
}

template<>
core::SoundHandle core::ResourceManager::LoadResource<core::SoundResource>(const str::Path& filepath)
{
	return m_SoundCache.load<core::SoundLoader>(entt::hashed_string{ filepath.ToChar() }, filepath);
}

template<>
core::TextureHandle core::ResourceManager::GetResource<core::TextureResource>(const str::Path& filepath)
{
	return m_TextureCache.handle(entt::hashed_string{ filepath.ToChar() });
}

template<>
core::TextureHandle core::ResourceManager::LoadResource<core::TextureResource>(const str::Path& filepath)
{
	return m_TextureCache.load<core::TextureLoader>(entt::hashed_string{ filepath.ToChar() }, filepath);
}
