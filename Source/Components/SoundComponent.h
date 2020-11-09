#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace audio
{
	struct SoundComponent
	{
		sf::Sound* m_Sound;
	};
}