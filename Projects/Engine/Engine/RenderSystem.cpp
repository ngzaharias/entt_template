#include "Engine/RenderSystem.h"

#include "Engine/CameraComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/VectorHelpers.h"

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
	const auto cameraView = registry.view<core::CameraComponent, core::TransformComponent>();
	for (const entt::entity& cameraEntity : cameraView)
	{
		// create the view from the camera
		{
			auto& camera = cameraView.get<core::CameraComponent>(cameraEntity);
			auto& transform = cameraView.get<core::TransformComponent>(cameraEntity);

			sf::Vector2f size = Multiply(camera.m_Size, { 1.f, -1.f });

			sf::View view;
			view.setCenter(transform.m_Translate.x, transform.m_Translate.y);
			view.setRotation(transform.m_Rotate.z);
			view.setSize(size);
			window->setView(view);
		}

		// render everything
		const auto renderView = registry.view<render::SpriteComponent, core::TransformComponent>();
		for (const entt::entity& renderEntity : renderView)
		{
			auto& sprite = renderView.get<render::SpriteComponent>(renderEntity);
			auto& transform = renderView.get<core::TransformComponent>(renderEntity);

			const sf::Texture* texture = sprite.m_Sprite.getTexture();
			const sf::Vector2f size = sf::Vector2f(texture->getSize());

			const float scaleX = sprite.m_Size.x / size.x;
			const float scaleY = sprite.m_Size.y / size.y;

			sprite.m_Sprite.setPosition(transform.m_Translate.x, transform.m_Translate.y);
			sprite.m_Sprite.setRotation(transform.m_Rotate.z);
			sprite.m_Sprite.setScale(transform.m_Scale.x * scaleX, transform.m_Scale.y * scaleY);

			window->draw(sprite.m_Sprite);
		}
	}
}
