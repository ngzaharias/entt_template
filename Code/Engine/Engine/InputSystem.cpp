#include "EnginePCH.h"
#include "Engine/InputSystem.h"

#include "Engine/InputComponent.h"

#include <entt/entity/registry.hpp>

void input::InputSystem::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& registry = m_World->m_Registry;

	m_KeysPrevious = m_KeysCurrent;
	for (int32 i = sf::Keyboard::Key::A; i < sf::Keyboard::Key::KeyCount; ++i)
	{
		const sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(i);
		if (sf::Keyboard::isKeyPressed(key))
			m_KeysCurrent.insert(key);
	}

	for (auto& entity : registry.view<input::InputComponent>())
	{
		auto& component = registry.get<input::InputComponent>(entity);
		component.m_KeysPrevious = m_KeysPrevious;
		component.m_KeysCurrent = m_KeysCurrent;
	}
}