#pragma once

namespace core
{
	struct SelectFileSettings
	{
		str::String m_Title = "Select File(s)";
		str::String m_Directory = "";
		std::vector<str::String> m_Filters = { "All Files (*.*)", "*" };
		bool m_IsMultiSelect = false;
	};

	struct SelectFolderSettings
	{
		str::String m_Title = "Select Folder";
		str::String m_Directory = "";
	};

	bool LoadFileAsBinary(const str::Path& filepath, str::String& out_String);

	std::vector<str::Path> SelectFileDialog(const SelectFileSettings& settings);
	str::Path SelectFolderDialog(const SelectFolderSettings& settings);
}