#pragma once

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

// #todo: implement asset browser in a pop-up
// #todo: copy and highlight for name and guid

template<>
void widget::TypeAsIs(physics::MaterialHandle& value)
{
	if (value)
	{
		const physics::MaterialResource& resource = value.get();

		ImGui::Text("Asset Name: %s", "...");
		ImGui::Text("Asset GUID: %s", resource.m_Guid.ToChar());
	}
	else
	{
		ImGui::Text("Asset Name: %s", "None");
		ImGui::Text("Asset GUID: %s", "None");
	}
}

template<>
void widget::TypeAsIs(audio::SoundHandle& value)
{
	if (value)
	{
		const audio::SoundResource& resource = value.get();

		// #todo: replace SourceFile.Name with AssetPath.Name
		ImGui::Text("Asset Name: %s", resource.m_SourceFile.GetFileNameNoExtension().data());
		ImGui::Text("Asset GUID: %s", resource.m_Guid.ToChar());
	}
	else
	{
		ImGui::Text("Asset Name: %s", "None");
		ImGui::Text("Asset GUID: %s", "None");
	}
}

template<>
void widget::TypeAsIs(render::TextureHandle& value)
{
	const Vector2f textureSize = Vector2f(100.f, 100.f);

	if (value)
	{
		const render::TextureResource& resource = value.get();

		// #todo: replace SourceFile.Name with AssetPath.Name
		ImGui::Text("Asset Name: %s", resource.m_SourceFile.GetFileNameNoExtension().data());
		ImGui::Text("Asset GUID: %s", resource.m_Guid.ToChar());
		ImGui::Image(resource.m_Texture, textureSize);
	}
	else
	{
		ImGui::Text("Asset Name: %s", "None");
		ImGui::Text("Asset GUID: %s", "None");
	}
}
