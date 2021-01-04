#pragma once

#include "Editor/AssetPopup.h"

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

// #todo: implement asset browser in a pop-up
// #todo: copy and highlight for name and guid

template<typename Type>
void widget::TypeAsIs(core::AssetHandle<Type>& handle)
{
	editor::AssetPopup::SelectOne(handle);
}

template<>
void widget::TypeAsIs(render::TextureHandle& handle)
{
	editor::AssetPopup::SelectOne(handle);

	//if (handle)
	//{
	//	const Vector2f textureSize = Vector2f(100.f, 100.f);
	//	const render::TextureAsset& Asset = handle.get();
	//	ImGui::Image(Asset.m_Texture, textureSize);
	//}
}
