#pragma once

#include <Engine/AssetHandle.h>
#include <Engine/AttributeTypes.h>
#include <Engine/TextureAsset.h>

namespace widget
{
	template<typename Type>
	void TypeOverload(core::AssetHandle<Type>& handle, const Attributes attributes = {});

	void TypeOverload(render::TextureHandle& handle, const Attributes attributes = {});
}

#include "AssetWidgets.inl"