#pragma once

#include <Engine/Hash.h>
#include <Engine/Name.h>
#include <Engine/String.h>
#include <Engine/StringView.h>

#include <map>

namespace str
{
	class NameTable final
	{
	public:
		static NameTable& Instance();

		NameTable();
		~NameTable();

		core::Hash Register(const char* string);
		core::Hash Register(const str::String& string);
		core::Hash Register(const str::StringView& string);

		const str::String& Retrieve(const core::Hash& hash) const;

	private:
		std::map<core::Hash, str::String> m_Values;
	};
}