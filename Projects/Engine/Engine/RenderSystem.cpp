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

render::RenderSystem::RenderSystem(sf::RenderTarget& renderTarget)
	: m_RenderTarget(renderTarget)
{
}

render::RenderSystem::~RenderSystem()
{
}

void render::RenderSystem::Update(entt::registry& registry, const core::GameTime& /*gameTime*/)
{
	const auto cameraView = registry.view<core::CameraComponent, core::TransformComponent>();
	for (const entt::entity& cameraEntity : cameraView)
	{
		// camera
		{
			auto& cameraComponent = cameraView.get<core::CameraComponent>(cameraEntity);
			auto& transformComponent = cameraView.get<core::TransformComponent>(cameraEntity);

			// #note: invert the size so that the coordinates of SFML match Engine
			const sf::Vector2f size = math::Multiply(cameraComponent.m_Size, { 1.f, -1.f });

			sf::View view;
			view.setCenter(transformComponent.m_Translate.x, transformComponent.m_Translate.y);
			view.setRotation(transformComponent.m_Rotate.z);
			view.setSize(size);
			m_RenderTarget.setView(view);
		}

		// sprite
		{
			const auto spriteView = registry.view<render::SpriteComponent, core::TransformComponent>();
			for (const entt::entity& renderEntity : spriteView)
			{
				auto& spriteComponent = spriteView.get<render::SpriteComponent>(renderEntity);
				auto& transformComponent = spriteView.get<core::TransformComponent>(renderEntity);
				
				if (!spriteComponent.m_Sprite)
					continue;

				const render::SpriteAsset& spriteAsset = spriteComponent.m_Sprite.get();
				if (!spriteAsset.m_Texture)
					continue;

				const render::TextureAsset& textureAsset = spriteAsset.m_Texture.get();

				const sf::Texture& texture = textureAsset.m_Texture;
				const sf::Vector2f size = sf::Vector2f(texture.getSize());

				const float scaleX = spriteComponent.m_Size.x / size.x;
				const float scaleY = spriteComponent.m_Size.y / size.y;

				sf::Sprite sprite;
				sprite.setPosition(transformComponent.m_Translate.x, transformComponent.m_Translate.y);
				sprite.setRotation(transformComponent.m_Rotate.z);
				sprite.setScale(transformComponent.m_Scale.x * scaleX, transformComponent.m_Scale.y * scaleY);
				sprite.setTexture(texture);
				sprite.setTextureRect(sf::IntRect
				(
					spriteAsset.m_Position.x
					, spriteAsset.m_Position.y
					, spriteAsset.m_Size.x
					, spriteAsset.m_Size.y
				));

				m_RenderTarget.draw(sprite);
			}
		}

		// flipbook
		{
			const auto flipbookView = registry.view<render::FlipbookComponent, core::TransformComponent>();
			for (const entt::entity& renderEntity : flipbookView)
			{
				auto& flipbookComponent = flipbookView.get<render::FlipbookComponent>(renderEntity);
				auto& transformComponent = flipbookView.get<core::TransformComponent>(renderEntity);

				const render::FlipbookAsset& flipbookAsset = flipbookComponent.m_Flipbook.get();
				if (flipbookAsset.m_Frames.empty())
					continue;

				const render::FlipbookFrame& flipbookFrame = flipbookAsset.m_Frames[flipbookComponent.m_Index];
				if (!flipbookFrame.m_Sprite)
					continue;

				const render::SpriteAsset& spriteAsset = flipbookFrame.m_Sprite.get();
				if (!spriteAsset.m_Texture)
					continue;

				const render::TextureAsset& textureAsset = spriteAsset.m_Texture.get();

				const sf::Texture& texture = textureAsset.m_Texture;

				sf::Sprite sprite;
				//sprite.setOrigin(size * 0.5f);
				sprite.setPosition(transformComponent.m_Translate.x, transformComponent.m_Translate.y);
				sprite.setRotation(transformComponent.m_Rotate.z);
				//sprite.setScale(transformComponent.m_Scale.x * scaleX, transformComponent.m_Scale.y * scaleY);
				sprite.setTexture(texture);
				sprite.setTextureRect(sf::IntRect
				(
					spriteAsset.m_Position.x
					, spriteAsset.m_Position.y
					, spriteAsset.m_Size.x
					, spriteAsset.m_Size.y
				));

				m_RenderTarget.draw(sprite);
			}
		}
	}
}
