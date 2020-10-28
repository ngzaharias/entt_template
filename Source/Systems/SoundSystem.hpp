#pragma once

#include "CircularBuffer.hpp"
#include "ObjectPool.hpp"

#include "Components/Sound.hpp"

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
	};

	class SoundSystem
	{
	public:
		SoundSystem();
		~SoundSystem();

		void Initialize(entt::registry& registry);
		void Destroy(entt::registry& registry);

		void Update(entt::registry& registry, const sf::Time& time);

		void PlaySound();
	private:
		std::vector<audio::Request> m_Requests;

		ObjectPool<sf::Sound, 128> m_SoundPool;
	};
};