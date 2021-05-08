#pragma once

#include <filesystem>

namespace str
{
	using Path = std::filesystem::path;

	//class Path final
	//{

	//public:
	//	Path();
	//	Path(const char* value);
	//	Path(const str::String& value);
	//	Path(const str::StringView& value);

	//	void operator=(const char* rhs);
	//	void operator=(const str::Path& rhs);
	//	void operator=(const str::String& rhs);
	//	void operator=(const str::StringView& rhs);

	//	void operator+=(const str::Path& rhs);
	//	void operator+=(const str::StringView& rhs);

	//	bool HasFileExtension(const str::StringView& rhs) const;
	// 
	//	bool IsDirectory() const;
	//	bool IsEmpty() const { return m_Value == ""; }
	//	bool IsFile() const;

	//	const char* ToChar() const { return m_Value.c_str(); }

	//	///		/My/Example/Directory/Foo.bar
	//	///		++++++++++++++++++++++
	//	str::StringView GetDirectory() const;

	//	///		/My/Example/Directory/Foo.bar
	//	///								 ++++
	//	str::StringView GetFileExtension() const;

	//	///		/My/Example/Directory/Foo.bar
	//	///							  +++++++
	//	str::StringView GetFileName() const;

	//	///		/My/Example/Directory/Foo.bar
	//	///							  +++
	//	str::StringView GetFileNameNoExtension() const;

	//	///		/My/Example/Directory/Foo.bar
	//	///		+++++++++++++++++++++++++
	//	str::StringView GetPathNoExtension() const;

	//private:
	//	str::String m_Value;
	//};
}