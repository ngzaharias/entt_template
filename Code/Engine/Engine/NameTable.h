#pragma once

#include <Engine/Hash.h>

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
		Map<core::Hash, str::String> m_Values;
	};
}