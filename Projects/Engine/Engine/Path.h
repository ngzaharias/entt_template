#pragma once

#include <string>

namespace str
{
	class Path final
	{

	public:
		Path();
		Path(const char* string);
		Path(const std::string_view& string);

		void operator=(const char* string);
		void operator=(const std::string_view& string);

		const char* ToChar() const { return m_Value.c_str(); }
		const std::string& ToString() const { return m_Value; }

		///		/My/Example/Directory/Foo.bar
		///		++++++++++++++++++++++
		std::string_view GetDirectory() const;

		///		/My/Example/Directory/Foo.bar
		///								 ++++
		std::string_view GetFileExtension() const;

		///		/My/Example/Directory/Foo.bar
		///							  +++++++
		std::string_view GetFileName() const;

		///		/My/Example/Directory/Foo.bar
		///							  +++
		std::string_view GetFileNameNoExtension() const;

	private:
		std::string m_Value;
	};
}