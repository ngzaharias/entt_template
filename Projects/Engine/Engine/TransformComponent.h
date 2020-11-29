#pragma once

#include <entt/entt.hpp>
#include <SFML/System/Vector3.hpp>

namespace core
{
	struct TransformComponent
	{
		sf::Vector3f m_Translate = { 0.f, 0.f, 0.f };
		sf::Vector3f m_Rotate = { 0.f, 0.f, 0.f };
		sf::Vector3f m_Scale = { 1.f, 1.f, 1.f };
	};
}
