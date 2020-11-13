#pragma once

#include <Engine/CircularBuffer.h>
#include <Engine/ObjectPool.h>
#include <Engine/Path.h>
#include <Engine/ResourceManager.h>
#include <Engine/SoundComponent.h>
#include <Engine/System.h>

#include <string>
#include <vector>
#include <entt/fwd.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace sf
{
	class Sound;
	class Time;
}

namespace audio
{
	struct Request
	{
		const str::Path& m_Filepath;
	};

	class SoundSystem final : public core::System
	{
	public:
		SoundSystem(core::ResourceManager& resourceManager);
		~SoundSystem();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const sf::Time& time) override;

		void PlaySound(const str::Path& filepath);

	private:
		core::ResourceManager& m_ResourceManager;

		std::vector<audio::Request> m_Requests;
		ObjectPool<sf::Sound, 128> m_SoundPool;
	};
};