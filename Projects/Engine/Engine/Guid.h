#pragma once

#include <string>
#include <entt/core/hashed_string.hpp>

namespace str
{
	class Guid final
	{
	public:
		Guid();
		Guid(const char* string);
		Guid(const std::string_view& string);

		bool operator<(const Guid& rhs) const { return m_Value.value() < rhs.m_Value.value(); }

		const char* ToChar() const { return m_Value.data(); }
		const entt::id_type ToHash() const { return m_Value.value(); }

	private:
		entt::hashed_string m_Value;
	};
}