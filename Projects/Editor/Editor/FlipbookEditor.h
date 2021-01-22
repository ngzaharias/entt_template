#pragma once

#include <Engine/FlipbookAsset.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/System.h>

namespace editor
{
	class FlipbookEditor final : public core::System
	{
	public:
		FlipbookEditor();
		~FlipbookEditor();

		void Initialize(entt::registry& registry) override;
		void Destroy(entt::registry& registry) override;

		void Update(entt::registry& registry, const core::GameTime& gameTime) override;

		void OpenEditor(const str::Name& guid);
		void CloseEditor();

	private:
		void Render();
		void Render_Playback();
		void Render_Preview();

	private:
		std::optional<render::FlipbookComponent> m_Component;
		bool m_IsLooping = true;
		bool m_IsPlaying = true;
	};
}
