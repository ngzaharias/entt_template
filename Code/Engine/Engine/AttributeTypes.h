#pragma once

#include <optional>
#include <refl/refl.hpp>

namespace attr
{
	struct Name : refl::attr::usage::field
	{
		constexpr Name() : m_Value(nullptr) { }
		constexpr Name(const char* name) : m_Value(name) { }
		const char* m_Value = "";
	};

	struct Range : refl::attr::usage::field
	{
		using Optionalf = std::optional<float>;

		constexpr Range() { }
		constexpr Range(const Optionalf min, const Optionalf max)
			: m_Min(min), m_Max(max) { }
		Optionalf m_Min = { };
		Optionalf m_Max = { };
	};

	struct Replicated : refl::attr::usage::field { };
}

namespace widget
{
	struct Attributes
	{
		std::optional<attr::Name> m_Name = { };
		std::optional<attr::Range> m_Range = { };
	};
}