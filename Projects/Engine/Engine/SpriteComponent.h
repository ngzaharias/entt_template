#pragma once

#include <Engine/TextureAsset.h>

#include <SFML/Graphics/Sprite.hpp>

namespace render
{
	struct SpriteComponent
	{
		Vector2f m_Size = Vector2f(1.f, 1.f);
		sf::Sprite m_Sprite = sf::Sprite();
	};
}