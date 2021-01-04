#include "Engine/EnginePCH.h"
#include "Engine/FileHelpers.h"

#include <PFD/portable-file-dialogs.h>

#include <fstream>
#include <sstream>

bool core::LoadFileAsBinary(const str::Path& filepath, str::String& out_String)
{
	std::ifstream is;
	std::ostringstream stream;
	is.open(filepath.ToChar(), std::ios::binary);
	if (!is.is_open())
		return false;

	stream << is.rdbuf();
	is.close();

	out_String = stream.str();
	return true;
}

std::vector<str::Path> core::SelectFileDialog(const SelectFileSettings& settings)
{
	pfd::open_file openFile = pfd::open_file
	(
		settings.m_Title
		, settings.m_Directory
		, settings.m_Filters
		, settings.m_IsMultiSelect ? pfd::opt::multiselect : pfd::opt::none
	);

	std::vector<str::Path> filepaths;
	for (const auto& name : openFile.result())
		filepaths.push_back(name);
	return filepaths;
}

str::Path core::SelectFolderDialog(const SelectFolderSettings& settings)
{
	pfd::select_folder selectFile = pfd::select_folder
	(
		settings.m_Title
		, settings.m_Directory
	);

	return selectFile.result();
}
