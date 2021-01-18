#pragma once

#include <Editor/TrivialWidgets.h>

#include <Engine/AssetHandle.h>
#include <Engine/TextureAsset.h>

namespace widget
{
	template<typename Type>
	void TypeAsIs(core::AssetHandle<Type>& handle);

	void TypeAsIs(render::TextureHandle& handle);
}

#include "AssetWidgets.inl"