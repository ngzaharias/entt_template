#pragma once

#include <Engine/System.h>

#include <SFML/Window/Keyboard.hpp>

namespace input
{
	class InputSystem final : ecs::System
	{
	public:
		InputSystem();
		~InputSystem();

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;

	private:
		Set<sf::Keyboard::Key> m_KeysPrevious;
		Set<sf::Keyboard::Key> m_KeysCurrent;
	};
}