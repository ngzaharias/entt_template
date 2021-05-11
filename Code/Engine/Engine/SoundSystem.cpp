#include "EnginePCH.h"
#include "Engine/SoundSystem.h"

#include "Engine/NameComponent.h"
#include "Engine/Random.h"
#include "Engine/AssetManager.h"
#include "Engine/SoundComponent.h"
#include "Engine/SoundAsset.h"
#include "Engine/TransformComponent.h"

#include <entt/entt.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

// #todo: load volume from config

audio::SoundSystem::SoundSystem(core::AssetManager& assetManager)
	: m_AssetManager(assetManager)
{
}

audio::SoundSystem::~SoundSystem()
{
}

void audio::SoundSystem::Initialise()
{
}

void audio::SoundSystem::Destroy()
{
}

void audio::SoundSystem::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& registry = m_World->m_Registry;

	for (const audio::Request& request : m_Requests)
	{
		const audio::SoundHandle handle = m_AssetManager.LoadAsset<audio::SoundAsset>(request.m_Guid);
		if (!handle)
			continue;

		ecs::Entity entity = registry.create();
		auto& name = registry.emplace<core::NameComponent>(entity);
		auto& sound = registry.emplace<audio::SoundComponent>(entity);
		auto& transform = registry.emplace<core::TransformComponent>(entity);

		name.m_Name = request.m_Guid.ToChar();
		sound.m_Handle = handle;
		sound.m_Sound = m_SoundPool.RequestObject();
		sound.m_Sound->setBuffer(handle->m_SoundBuffer);
		sound.m_Sound->setVolume(m_Volume);
		sound.m_Sound->play();
	}
	m_Requests.clear();

	const auto view = registry.view<audio::SoundComponent>();
	for (const ecs::Entity& entity : view)
	{
		auto& sound = registry.get<audio::SoundComponent>(entity);
		if (sound.m_Sound->getStatus() == sf::Sound::Stopped)
		{
			m_SoundPool.ReleaseObject(sound.m_Sound);
			registry.destroy(entity);
		}
	}
}

void audio::SoundSystem::PlaySound(const str::Guid& guid)
{
	m_Requests.push_back({ guid });
}

