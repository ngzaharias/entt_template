#pragma once

#include <set>
#include <SFML/Window/Keyboard.hpp>

namespace core
{
	struct InputComponent
	{
		bool IsKeyHeld(sf::Keyboard::Key key) const
		{
			return m_KeysCurrent.count(key) > 0;
		}

		bool IsKeyPressed(sf::Keyboard::Key key) const
		{
			return m_KeysPrevious.count(key) == 0
				&& m_KeysCurrent.count(key) > 0;
		}

		bool IsKeyReleased(sf::Keyboard::Key key) const
		{
			return m_KeysPrevious.count(key) > 0
				&& m_KeysCurrent.count(key) == 0;
		}

		std::set<sf::Keyboard::Key> m_KeysPrevious;
		std::set<sf::Keyboard::Key> m_KeysCurrent;
	};
}

REFL_AUTO
(
	type(core::InputComponent)
	, field(m_KeysPrevious)
	, field(m_KeysCurrent)
)