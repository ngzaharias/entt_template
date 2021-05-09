#include "EnginePCH.h"
#include "Engine/AssetHelpers.h"

#include <entt/core/type_info.hpp>

template<class Type>
core::EAssetType core::ToAssetType()
{
	constexpr core::TypeId typeId = core::ToTypeId<Type>();
	return static_cast<core::EAssetType>(typeId);
}
