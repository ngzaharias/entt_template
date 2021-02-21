#pragma once

#include <Engine/String.h>

namespace core
{
	struct LevelComponent
	{
		str::String m_Name = { };
		str::String m_Path = { };
	};
}

REFL_AUTO
(
	type(core::LevelComponent)
	, field(m_Name)
	, field(m_Path)
)