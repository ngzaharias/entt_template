#pragma once

namespace core
{
	struct CameraComponent
	{
		Vector2f m_Size;
	};
}

REFL_AUTO
(
	type(core::CameraComponent)
	, field(m_Size)
)