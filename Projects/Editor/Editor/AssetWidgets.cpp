#include "Editor/EditorPCH.h"
#include "Editor/AssetWidgets.h"

#include "Editor/AssetPopup.h"

#include <imgui-sfml/imgui-SFML.h>

void widget::TypeOverload(render::TextureHandle& handle, const Properties properties /*= {}*/)
{
	editor::AssetPopup::SelectOne(handle);

	//if (handle)
	//{
	//	const Vector2f textureSize = Vector2f(100.f, 100.f);
	//	const render::TextureAsset& asset = handle.get();
	//	ImGui::Image(asset.m_Texture, textureSize);
	//}
}
