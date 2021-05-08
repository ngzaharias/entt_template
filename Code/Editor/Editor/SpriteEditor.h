#pragma once

#include <Engine/SpriteAsset.h>
#include <Engine/System.h>

namespace editor
{
	class SpriteEditor final : public ecs::System
	{
	public:
		SpriteEditor();
		~SpriteEditor();

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;

		void OpenEditor(const str::Name& guid);
		void CloseEditor();

	private:
		void Render();

	private:
		render::SpriteHandle m_Sprite = { };
	};
}
