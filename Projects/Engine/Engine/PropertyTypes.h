#pragma once

#include <optional>
#include <refl/refl.hpp>

namespace prop
{
	struct Name : refl::attr::usage::field
	{
		constexpr Name() : m_Value(nullptr) { }
		constexpr Name(const char* name) : m_Value(name) { }
		const char* m_Value = "";
	};

	struct Range : refl::attr::usage::field
	{
		constexpr Range()
			: m_Min(0.f), m_Max(0.f) { }
		constexpr Range(const float min, const float max)
			: m_Min(min), m_Max(max) { }
		constexpr Range(const int32 min, const int32 max)
			: m_Min(static_cast<float>(min)), m_Max(static_cast<float>(max)) { }
		float m_Min = 0.f;
		float m_Max = 0.f;
	};

}

namespace widget
{
	struct Properties
	{
		std::optional<prop::Name> m_Name = { };
		std::optional<prop::Range> m_Range = { };
	};
}