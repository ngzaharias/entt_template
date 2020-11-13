#pragma once

#include <Engine/SoundResource.h>

#include <SFML/Audio/Sound.hpp>

namespace audio
{
	struct SoundComponent
	{
		audio::SoundHandle m_Handle;
		sf::Sound* m_Sound;
	};
}