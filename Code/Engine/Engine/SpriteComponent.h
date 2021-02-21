#pragma once

#include <Engine/SpriteAsset.h>

namespace render
{
	struct SpriteComponent
	{
		Vector2u m_Size = { 0, 0 };
		SpriteHandle m_Sprite = { };
	};
}

REFL_AUTO
(
	type(render::SpriteComponent)
	, field(m_Size)
	, field(m_Sprite)
)