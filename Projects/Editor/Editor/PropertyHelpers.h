#pragma once

#include <entt/meta/meta.hpp>

namespace editor
{
	const char* PropertyName(const entt::meta_data& metaData, const char* _default = nullptr);
	const char* PropertyName(const entt::meta_type& metaType, const char* _default = nullptr);

	size_t PropertyOffset(const entt::meta_data& metaData, const size_t _default = 0);
	size_t PropertyStride(const entt::meta_data& metaData, const size_t _default = 0);
}