#pragma once

#include <Engine/ObjectPool.h>
#include <Engine/Path.h>
#include <Engine/System.h>

#include <SFML/Audio/Sound.hpp>

namespace core
{
	class AssetManager;
}

namespace sf
{
	class Time;
}

namespace audio
{
	struct Request
	{
		const str::Guid& m_Guid;
	};

	class SoundSystem final : public ecs::System
	{
	public:
		SoundSystem(core::AssetManager& assetManager);
		~SoundSystem();

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;

		void PlaySound(const str::Guid& guid);

		float GetVolume() const { return m_Volume; }
		void SetVolume(float value) { m_Volume = value; }

	private:
		core::AssetManager& m_AssetManager;

		Array<audio::Request> m_Requests;
		ObjectPool<sf::Sound, 128> m_SoundPool;

		float m_Volume = 0.f;
	};
};