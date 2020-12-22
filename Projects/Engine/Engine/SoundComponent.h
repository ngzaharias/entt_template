#pragma once

#include <Engine/SoundResource.h>

#include <SFML/Audio/Sound.hpp>

namespace audio
{
	struct SoundComponent
	{
		audio::SoundPtr m_Handle;
		sf::Sound* m_Sound;
	};
}