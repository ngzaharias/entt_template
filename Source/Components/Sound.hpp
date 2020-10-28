#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace audio
{
	struct Sound
	{
		sf::Sound* m_Sound;
	};
}