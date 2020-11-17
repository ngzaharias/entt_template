#pragma once

#include <Engine/Hash.h>
#include <Engine/String.h>
#include <Engine/StringView.h>

#define NAME(str) str::Name::Create(str)

namespace str
{
	class Name final
	{
	public:
		static constexpr core::Hash Unassigned = -1;

		Name() : m_Hash(Unassigned) { }

		bool operator<(const str::Name& rhs) const { return m_Hash < rhs.m_Hash; }

		const bool IsEmpty() const;

		const char* ToChar() const;
		const core::Hash ToHash() const { return m_Hash; }
		const str::String& ToString() const;

		static str::Name Create(const char* string);
		static str::Name Create(const str::String& string);
		static str::Name Create(const str::StringView& string);

	private:
		core::Hash m_Hash;
	};
}