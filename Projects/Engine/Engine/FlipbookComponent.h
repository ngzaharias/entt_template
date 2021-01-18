#pragma once

#include <Engine/FlipbookAsset.h>

namespace render
{
	struct FlipbookComponent
	{
		float m_Time = 0.f;
		uint32 m_Index = 0;
		Vector2u m_Size = { 0, 0 };

		FlipbookHandle m_Flipbook = { };
	};
}