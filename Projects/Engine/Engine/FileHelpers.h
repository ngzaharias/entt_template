#pragma once

#include <Engine/Path.h>
#include <Engine/String.h>

namespace core
{
	bool LoadFileAsBinary(const str::Path& filepath, str::String& out_String);
}