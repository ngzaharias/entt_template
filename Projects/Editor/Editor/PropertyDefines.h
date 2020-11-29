#pragma once

#include <entt/core/hashed_string.hpp>

namespace editor
{
	const entt::hashed_string strCustom = "Custom"_hs;
	const entt::hashed_string strMetaId = "MetaId"_hs;
	const entt::hashed_string strName = "Name"_hs;
	const entt::hashed_string strOffset = "Offset"_hs;
	const entt::hashed_string strStride = "Stride"_hs;

	struct Void { void* p; };
}