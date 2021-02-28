#pragma once

#include <Editor/InspectorTypes.h>

#include <Engine/AssetHandle.h>
#include <Engine/TextureAsset.h>

namespace editor
{
	template<typename Type>
	void TypeOverload(core::AssetHandle<Type>& handle, const Attributes attributes = {});

	void TypeOverload(render::TextureHandle& handle, const Attributes attributes = {});
}

#include "AssetWidgets.inl"