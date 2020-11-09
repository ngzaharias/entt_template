#pragma once

#include <functional>
#include <string>

#define NAME(str) string::Name::Create(str)

namespace string
{
	using Hash = size_t;

	class Name final
	{
	public:
		static constexpr Hash Unassigned = -1;

		const bool IsEmpty() const;

		const char* ToChar() const;

		static Name Create(const char* string);
		static Name Create(const std::string_view& string);

	private:
		Name() : m_Hash(Unassigned) { }

	private:
		Hash m_Hash;
	};
}