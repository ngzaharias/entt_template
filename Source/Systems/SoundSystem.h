#pragma once

#include <Containers/CircularBuffer.h>
#include <Containers/ObjectPool.h>
#include <Components/SoundComponent.h>
#include <Resources/ResourceManager.h>
#include <Strings/Path.h>

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

	class SoundSystem
	{
	public:
		SoundSystem();
		~SoundSystem();

		void Initialize(entt::registry& registry);
		void Destroy(entt::registry& registry);

		void Update(entt::registry& registry, const sf::Time& time);

		void PlaySound(const str::Path& filepath);

	private:
		std::vector<audio::Request> m_Requests;
		ObjectPool<sf::Sound, 128> m_SoundPool;
	};
};