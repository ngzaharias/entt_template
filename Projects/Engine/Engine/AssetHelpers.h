#pragma once

#include <Engine/AssetTypes.h>

namespace core
{
	template<class TAsset>
	core::EAssetType ToAssetType();
	core::EAssetType ToAssetType(const str::StringView& string);
	const char* ToAssetType(core::EAssetType type);
}

#include "AssetHelpers.inl"