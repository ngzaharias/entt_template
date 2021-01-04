#pragma once

#include <Editor/TrivialWidgets.h>

#include <Engine/EntityTemplateAsset.h>
#include <Engine/PhysicsMaterialAsset.h>
#include <Engine/SoundAsset.h>
#include <Engine/TextureAsset.h>

namespace widget
{
	template<typename Type>
	void TypeAsIs(core::AssetHandle<Type>& handle);

	template<>
	void TypeAsIs(render::TextureHandle& handle);
}

#include "AssetWidgets.inl"