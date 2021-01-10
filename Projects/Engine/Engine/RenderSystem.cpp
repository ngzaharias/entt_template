#include "Engine/EnginePCH.h"
#include "Engine/RenderSystem.h"

#include "Engine/CameraComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/VectorHelpers.h"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

// #todo: render a sprite and a flipbook based on their z-position

render::RenderSystem::RenderSystem(sf::RenderWindow& window)
	: m_Window(window)
{
}

render::RenderSystem::~RenderSystem()
{
}

void render::RenderSystem::Update(entt::registry& registry, const sf::Time& time)
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
			m_Window.setView(view);
		}

		{
			const auto spriteView = registry.view<render::SpriteComponent, core::TransformComponent>();
			for (const entt::entity& renderEntity : spriteView)
			{
				auto& sprite = spriteView.get<render::SpriteComponent>(renderEntity);
				auto& transform = spriteView.get<core::TransformComponent>(renderEntity);

				const sf::Texture* texture = sprite.m_Sprite.getTexture();
				const sf::Vector2f size = sf::Vector2f(texture->getSize());

				const float scaleX = sprite.m_Size.x / size.x;
				const float scaleY = sprite.m_Size.y / size.y;

				sprite.m_Sprite.setPosition(transform.m_Translate.x, transform.m_Translate.y);
				sprite.m_Sprite.setRotation(transform.m_Rotate.z);
				sprite.m_Sprite.setScale(transform.m_Scale.x * scaleX, transform.m_Scale.y * scaleY);

				m_Window.draw(sprite.m_Sprite);
			}
		}

		{
			const auto flipbookView = registry.view<render::FlipbookComponent, core::TransformComponent>();
			for (const entt::entity& renderEntity : flipbookView)
			{
				auto& flipbook = flipbookView.get<render::FlipbookComponent>(renderEntity);
				auto& transform = flipbookView.get<core::TransformComponent>(renderEntity);

				const sf::Texture* texture = flipbook.m_Sprite.getTexture();

				flipbook.m_Time += time.asSeconds();
				flipbook.m_Index = static_cast<uint32>(flipbook.m_Time * flipbook.m_FPS);
				flipbook.m_Index %= flipbook.m_SubSprite.m_Count;

				const Vector2u textureSize = texture->getSize();
				const Vector2u dimensions =
				{
					textureSize.x / flipbook.m_SubSprite.m_Size.x,
					textureSize.y / flipbook.m_SubSprite.m_Size.y,
				};

				const Vector2i subSize = Vector2i(flipbook.m_SubSprite.m_Size);
				const Vector2i subPosition =
				{
					static_cast<int32>(flipbook.m_Index % dimensions.x) * subSize.x,
					static_cast<int32>(flipbook.m_Index / dimensions.y) * subSize.y,
				};

				const float scaleX = flipbook.m_Size.x / subSize.x;
				const float scaleY = flipbook.m_Size.y / subSize.y;

				flipbook.m_Sprite.setTextureRect({ subPosition, subSize });
				flipbook.m_Sprite.setPosition(transform.m_Translate.x, transform.m_Translate.y);
				flipbook.m_Sprite.setRotation(transform.m_Rotate.z);
				flipbook.m_Sprite.setScale(transform.m_Scale.x * scaleX, transform.m_Scale.y * scaleY);

				m_Window.draw(flipbook.m_Sprite);

			}
		}
	}
}
