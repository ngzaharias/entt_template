#pragma once

#include <string>

namespace str
{
	class Path final
	{

	public:
		Path();
		Path(const char* string);
		Path(const str::StringView& string);

		void operator=(const char* string);
		void operator=(const str::StringView& string);

		bool HasFileExtension(const char* extension) const;
		bool IsDirectory() const;
		bool IsFile() const;

		const char* ToChar() const { return m_Value.c_str(); }
		const str::String& ToString() const { return m_Value; }
		const str::StringView& ToStringView() const { return m_Value; }

		///		/My/Example/Directory/Foo.bar
		///		++++++++++++++++++++++
		str::StringView GetDirectory() const;

		///		/My/Example/Directory/Foo.bar
		///								 ++++
		str::StringView GetFileExtension() const;

		///		/My/Example/Directory/Foo.bar
		///							  +++++++
		str::StringView GetFileName() const;

		///		/My/Example/Directory/Foo.bar
		///							  +++
		str::StringView GetFileNameNoExtension() const;

	private:
		str::String m_Value;
	};
}