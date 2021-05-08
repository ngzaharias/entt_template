#pragma once

#include <Engine/System.h>

#include <set>
#include <SFML/Window/Keyboard.hpp>

namespace input
{
	class InputSystem final : core::System
	{
	public:
		InputSystem();
		~InputSystem();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

	private:
		Set<sf::Keyboard::Key> m_KeysPrevious;
		Set<sf::Keyboard::Key> m_KeysCurrent;
	};
}