#pragma once

#include <functional>
#include <string>

#define NAME(str) str::Name::Create(str)

namespace str
{
	using Hash = size_t;

	class Name final
	{
	public:
		static constexpr Hash Unassigned = -1;

		Name() : m_Hash(Unassigned) { }

		bool operator<(const Name& rhs) const { return m_Hash < rhs.m_Hash; }

		const bool IsEmpty() const;

		const char* ToChar() const;
		const std::string& ToString() const;

		static Name Create(const char* string);
		static Name Create(const std::string_view& string);

	private:
		Hash m_Hash;
	};
}