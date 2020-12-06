#pragma once

#include "Engine/FieldAttributes.h"

#include <entt/entt.hpp>
#include <refl/refl.hpp>
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

REFL_AUTO
(
	type(core::TransformComponent)
	, field(m_Translate, field::Name("Translate"))
	, field(m_Rotate, field::Name("Rotate"))
	, field(m_Scale, field::Name("Scale"))
)