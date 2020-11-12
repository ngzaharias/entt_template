#pragma once

#include <Resources/TextureResource.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

namespace render
{
	struct SpriteComponent
	{
		core::TextureHandle m_Handle;
		sf::Vector2f m_Size = sf::Vector2f(1.f, 1.f);
		sf::Sprite m_Sprite = sf::Sprite();
	};
}