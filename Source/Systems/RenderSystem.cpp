#include "RenderSystem.hpp"

#include "Components/Camera.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

render::RenderSystem::RenderSystem()
{
}

render::RenderSystem::~RenderSystem()
{
}

void render::RenderSystem::Update(entt::registry& registry, sf::RenderWindow* window)
{
	const auto cameraView = registry.view<core::Camera, core::Transform>();
	for (const entt::entity& cameraEntity : cameraView)
	{
		// create the view from the camera
		{
			auto& camera = cameraView.get<core::Camera>(cameraEntity);
			auto& transform = cameraView.get<core::Transform>(cameraEntity);

			sf::View view;
			view.setCenter(transform.m_Translate);
			view.setRotation(transform.m_Rotate);
			view.setSize(camera.m_Size);
			window->setView(view);
		}

		// render everything
		const auto renderView = registry.view<render::Sprite, core::Transform>();
		for (const entt::entity& renderEntity : renderView)
		{
			auto& sprite = renderView.get<render::Sprite>(renderEntity);
			auto& transform = renderView.get<core::Transform>(renderEntity);

			sprite.m_Sprite.setPosition(transform.m_Translate);
			sprite.m_Sprite.setRotation(transform.m_Rotate);
			sprite.m_Sprite.setScale(transform.m_Scale);

			window->draw(sprite.m_Sprite);
		}
	}
}
