#pragma once

namespace core
{
	struct NameComponent
	{
		str::String m_Name;
	};
}

REFL_AUTO
(
	type(core::NameComponent)
	, field(m_Name)
)