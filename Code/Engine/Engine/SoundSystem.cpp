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

audio::SoundSystem::SoundSystem(core::AssetManager& assetManager)
	: m_AssetManager(assetManager)
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

void audio::SoundSystem::Update(entt::registry& registry, const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const audio::Request& request : m_Requests)
	{
		const audio::SoundHandle handle = m_AssetManager.LoadAsset<audio::SoundAsset>(request.m_Name);
		if (!handle)
			continue;

		entt::entity entity = registry.create();
		auto& name = registry.emplace<core::NameComponent>(entity);
		auto& sound = registry.emplace<audio::SoundComponent>(entity);
		auto& transform = registry.emplace<core::TransformComponent>(entity);

		name.m_Name = request.m_Name.ToChar();
		sound.m_Handle = handle;
		sound.m_Sound = m_SoundPool.RequestObject();
		sound.m_Sound->setBuffer(handle->m_SoundBuffer);
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

void audio::SoundSystem::PlaySound(const str::Name& name)
{
	m_Requests.push_back({ name });
}

