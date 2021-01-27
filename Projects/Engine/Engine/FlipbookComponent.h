#pragma once

#include <Engine/FlipbookAsset.h>

namespace render
{
	struct FlipbookComponent
	{
		FlipbookHandle m_Flipbook = { };
		Vector2u m_Size = { 100, 100 };
		int32 m_Index = 0;
		float m_FPS = 30.f;
		float m_Time = 0.f;
		bool m_IsLooping = true;
		bool m_IsPlaying = true;
	};
}

REFL_AUTO
(
	type(render::FlipbookComponent)
	, field(m_Flipbook)
	, field(m_Size)
	, field(m_Index, attr::Range(0.f, { }))
	, field(m_FPS, attr::Range(1.f, 100.f))
	, field(m_Time)
	, field(m_IsLooping)
	, field(m_IsPlaying)
)