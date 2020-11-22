#include "Engine/FileHelpers.h"

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
