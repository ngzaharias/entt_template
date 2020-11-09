#include "SoundSystem.hpp"

#include "Random.hpp"

#include "Components/NameComponent.h"
#include "Components/SoundComponent.h"
#include "Components/TransformComponent.h"

#include <iostream>
#include <entt/entt.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

audio::SoundSystem::SoundSystem()
{
}

audio::SoundSystem::~SoundSystem()
{
}

void audio::SoundSystem::Initialize(entt::registry& registry)
{
}

void audio::SoundSystem::Destroy(entt::registry& registry)
{
}

void audio::SoundSystem::Update(entt::registry& registry, const sf::Time& time)
{
	for (const audio::Request& request : m_Requests)
	{
		const char* soundName = "<unknown>";
		sf::SoundBuffer* soundBuffer = nullptr;
		if (!soundBuffer)
			continue;

		entt::entity entity = registry.create();
		auto& name = registry.emplace<debug::NameComponent>(entity);
		auto& sound = registry.emplace<audio::SoundComponent>(entity);
		auto& transform = registry.emplace<core::TransformComponent>(entity);

		name.m_Name = soundName;
		sound.m_Sound = m_SoundPool.RequestObject();
		sound.m_Sound->setBuffer(*soundBuffer);
		sound.m_Sound->setVolume(20.f);
		sound.m_Sound->play();
	}
	m_Requests.clear();

	const auto view = registry.view<audio::SoundComponent>();
	for (const entt::entity& entity : view)
	{
		auto& sound = registry.get<audio::SoundComponent>(entity);
		if (sound.m_Sound->getStatus() == sf::Sound::Stopped)
		{
			m_SoundPool.ReleaseObject(sound.m_Sound);
			registry.destroy(entity);
		}
	}
}

void audio::SoundSystem::PlaySound()
{
	m_Requests.push_back({});
}

