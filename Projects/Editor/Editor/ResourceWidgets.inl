#pragma once

#include "Editor/AssetPopup.h"

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

// #todo: implement asset browser in a pop-up
// #todo: copy and highlight for name and guid

template<>
void widget::TypeAsIs(physics::MaterialHandle& value)
{
	editor::AssetPopup::Resource(value);
}

template<>
void widget::TypeAsIs(audio::SoundHandle& value)
{
	editor::AssetPopup::Resource(value);
}

template<>
void widget::TypeAsIs(render::TextureHandle& value)
{
	editor::AssetPopup::Resource(value);

	//if (value)
	//{
	//	const Vector2f textureSize = Vector2f(100.f, 100.f);
	//	const render::TextureResource& resource = value.get();
	//	ImGui::Image(resource.m_Texture, textureSize);
	//}
}
