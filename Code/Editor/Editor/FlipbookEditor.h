#pragma once

#include <Engine/FlipbookAsset.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/System.h>

#include <entt/entity/entity.hpp>

namespace render
{
	struct FlipbookComponent;
}

namespace editor
{
	class FlipbookEditor final : public ecs::System
	{
	public:
		FlipbookEditor();
		~FlipbookEditor();

		void Initialise() override;
		void Destroy() override;

		void Update(const core::GameTime& gameTime) override;

		void OpenEditor(const str::Guid& guid);
		void CloseEditor();

	private:
		void Render(render::FlipbookComponent& component);
		void Render_Playback(render::FlipbookComponent& component);
		void Render_Preview(render::FlipbookComponent& component);

	private:
		entt::entity m_Entity = entt::null;
		Nullable<str::Guid> m_Guid = { };
	};
}
