#pragma once

#include "Name.hpp"

#include <map>
#include <string>

namespace string
{
	class NameTable final
	{
	public:
		static NameTable& Instance();

		NameTable();
		~NameTable();

		Hash Register(const char* string);
		Hash Register(const std::string_view& string);

		const std::string& Retrieve(const Hash& hash) const;

	private:
		std::map<Hash, std::string> m_Strings;
	};
}