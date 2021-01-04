#include "Engine/EnginePCH.h"
#include "Engine/AssetHelpers.h"

#include <entt/core/type_info.hpp>

template<class Type>
core::EAssetType core::ToAssetType()
{
	constexpr entt::id_type typeId = entt::type_info<Type>::id();
	return static_cast<core::EAssetType>(typeId);
}
