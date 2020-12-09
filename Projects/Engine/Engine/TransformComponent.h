#pragma once

#include <Engine/FieldAttributes.h>

namespace core
{
	struct TransformComponent
	{
		Vector3f m_Translate = { 0.f, 0.f, 0.f };
		Vector3f m_Rotate = { 0.f, 0.f, 0.f };
		Vector3f m_Scale = { 1.f, 1.f, 1.f };
	};
}

REFL_AUTO
(
	type(core::TransformComponent)
	, field(m_Translate, field::Name("Translate"))
	, field(m_Rotate, field::Name("Rotate"))
	, field(m_Scale, field::Name("Scale"))
)