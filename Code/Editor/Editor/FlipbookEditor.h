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
		void Render(render::FlipbookComponent& component);
		void Render_Playback(render::FlipbookComponent& component);
		void Render_Preview(render::FlipbookComponent& component);

	private:
		entt::entity m_Entity = entt::null;
		Nullable<str::Name> m_Guid = { };
	};
}
