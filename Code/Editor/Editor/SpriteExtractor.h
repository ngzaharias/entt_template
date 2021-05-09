#pragma once

#include <Engine/System.h>
#include <Engine/TextureAsset.h>

namespace editor
{
	struct NamingSettings
	{
		int32 m_StartIndex = 0;
		str::String m_Template = "SP_Sprite_";
	};

	struct GridSettings
	{
		Vector2u m_Count = { 0, 0 };
		Vector2u m_Margin = { 0, 0 };
		Vector2u m_Size = { 0, 0 };
		Vector2u m_Spacing = { 0, 0 };
	};

	class SpriteExtractor final : public ecs::System
	{
	public:
		SpriteExtractor();
		~SpriteExtractor();

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;

		void OpenDialog(const str::Guid& guid);
		void CloseDialog();

	private:
		void Render();

		bool ExtractDialog();

	private:
		render::TextureHandle m_Texture = { };

		NamingSettings m_NamingSettings;
		GridSettings m_GridSettings;
	};
}

REFL_AUTO
(
	type(editor::NamingSettings)
	, field(m_StartIndex)
	, field(m_Template)
)

REFL_AUTO
(
	type(editor::GridSettings)
	, field(m_Count)
	, field(m_Margin)
	, field(m_Size)
	, field(m_Spacing)
)