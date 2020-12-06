#pragma once

#include <refl/refl.hpp>

namespace field
{
	struct Name : refl::attr::usage::field
	{
		constexpr Name(const char* name) : m_Value(name) { }
		const char* m_Value = "";
	};
}