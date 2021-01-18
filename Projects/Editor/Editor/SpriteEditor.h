#pragma once

#include <Engine/SpriteAsset.h>
#include <Engine/System.h>

namespace editor
{
	class SpriteEditor final : public core::System
	{
	public:
		SpriteEditor();
		~SpriteEditor();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

		void OpenEditor(const str::Name& guid);
		void CloseEditor();

	private:
		void Render();

	private:
		render::SpriteHandle m_Sprite = { };
	};
}
