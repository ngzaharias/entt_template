#pragma once

#include <entt/entity/fwd.hpp>

namespace core
{
	using TypeId = entt::id_type;

	template<typename Type>
	constexpr core::TypeId ToTypeId();
}

#include "TypeId.inl"