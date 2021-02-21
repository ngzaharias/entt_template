#pragma once

#include <Engine/ObjectPool.h>
#include <Engine/Path.h>
#include <Engine/System.h>

#include <vector>
#include <entt/fwd.hpp>
#include <SFML/Audio/Sound.hpp>

namespace core
{
	class AssetManager;
}

namespace sf
{
	class Time;
}

namespace str
{
	class Name;
}

namespace audio
{
	struct Request
	{
		const str::Name& m_Name;
	};

	class SoundSystem final : public core::System
	{
	public:
		SoundSystem(core::AssetManager& assetManager);
		~SoundSystem();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

		void PlaySound(const str::Name& name);

	private:
		core::AssetManager& m_AssetManager;

		std::vector<audio::Request> m_Requests;
		ObjectPool<sf::Sound, 128> m_SoundPool;
	};
};