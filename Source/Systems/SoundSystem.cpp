#include "SoundSystem.h"

#include "Game.h"
#include "Random.h"
#include "Components/NameComponent.h"
#include "Components/SoundComponent.h"
#include "Components/TransformComponent.h"

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
	core::ResourceManager* resourceManager = Game::Instance().m_ResourceManager;

	for (const audio::Request& request : m_Requests)
	{
		const core::SoundHandle handle = resourceManager->GetResource<core::SoundResource>(request.m_Filepath);
		if (!handle)
			continue;

		entt::entity entity = registry.create();
		auto& name = registry.emplace<debug::NameComponent>(entity);
		auto& sound = registry.emplace<audio::SoundComponent>(entity);
		auto& transform = registry.emplace<core::TransformComponent>(entity);

		name.m_Name = request.m_Filepath.ToString();
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

void audio::SoundSystem::PlaySound(const str::Path& filepath)
{
	m_Requests.push_back({ filepath });
}

