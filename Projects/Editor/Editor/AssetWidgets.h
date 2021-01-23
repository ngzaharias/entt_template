#pragma once

#include <Engine/AssetHandle.h>
#include <Engine/PropertyTypes.h>
#include <Engine/TextureAsset.h>

namespace widget
{
	template<typename Type>
	void TypeOverload(core::AssetHandle<Type>& handle, const Properties properties = {});

	void TypeOverload(render::TextureHandle& handle, const Properties properties = {});
}

#include "AssetWidgets.inl"