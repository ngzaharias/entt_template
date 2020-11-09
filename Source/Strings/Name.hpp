#pragma once

#include <functional>
#include <string>

#define NAME(string) string::Name::Create(string)

namespace string
{
	using Hash = size_t;

	struct Name
	{
	private:
		Name();

	public:
		const char* ToChar() const;

		static Name Create(const char* string);
		static Name Create(const std::string_view& string);

	private:
		Hash m_Hash = -1;
	};
}