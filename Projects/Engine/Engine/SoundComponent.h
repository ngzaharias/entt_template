#pragma once

#include <Engine/SoundAsset.h>

#include <SFML/Audio/Sound.hpp>

namespace audio
{
	struct SoundComponent
	{
		audio::SoundHandle m_Handle;
		sf::Sound* m_Sound;
	};
}

REFL_AUTO
(
	type(audio::SoundComponent)
	, field(m_Handle)
	, field(m_Sound)
)