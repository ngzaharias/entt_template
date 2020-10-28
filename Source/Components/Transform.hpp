#pragma once

#include <SFML/System/Vector2.hpp>

namespace core
{
	struct Transform
	{
		sf::Vector2f m_Translate = { 0.f, 0.f };
		float m_Rotate = 0.f;
		sf::Vector2f m_Scale = { 1.f, 1.f };
	};
}