#pragma once

#include <Engine/FlipbookAsset.h>

namespace render
{
	struct FlipbookComponent
	{
		FlipbookHandle m_Flipbook = { };
		Vector2u m_Size = { 0, 0 };
		int32 m_Index = 0;
		float m_FPS = 0.f;
		float m_Time = 0.f;
		bool m_IsLooping = true;
		bool m_IsPlaying = true;
	};
}