#pragma once

#include <Engine/FlipbookAsset.h>
#include <Engine/TextureAsset.h>

#include <SFML/Graphics/Sprite.hpp>

namespace render
{
	struct SubSprite
	{
		uint32 m_Count = 0;
		Vector2u m_Size = { 0, 0 };
	};

	struct FlipbookComponent
	{
		float m_FPS = 0.f;
		float m_Time = 0.f;
		uint32 m_Index = 0;

		SubSprite m_SubSprite;

		Vector2f m_Size = Vector2f(1.f, 1.f);
		sf::Sprite m_Sprite = sf::Sprite();
	};
}