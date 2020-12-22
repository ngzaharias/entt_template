#pragma once

#include <Engine/Name.h>
#include <Engine/Path.h>

namespace core
{
	struct Resource
	{
		str::Name m_Guid;
		str::Path m_Filepath;
	};
}