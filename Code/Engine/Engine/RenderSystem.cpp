#include "EnginePCH.h"
#include "Engine/RenderSystem.h"

#include "Engine/CameraComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/Screen.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/VectorHelpers.h"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

// #todo: render sprites based on their z-position and not based on the order they were called
// #todo: error if there is no camera

render::RenderSystem::RenderSystem(sf::RenderTarget& renderTarget)
	: m_RenderTarget(renderTarget)
{
}

render::RenderSystem::~RenderSystem()
{
}

void render::RenderSystem::Update(entt::registry& registry, const core::GameTime& /*gameTime*/)
{
	PROFILE_FUNCTION();

	const auto cameraView = registry.view<core::CameraComponent, core::TransformComponent>();
	for (const entt::entity& cameraEntity : cameraView)
	{
		// camera
		{
			auto& cameraComponent = cameraView.get<core::CameraComponent>(cameraEntity);
			auto& transformComponent = cameraView.get<core::TransformComponent>(cameraEntity);

			sf::View view;
			view.setCenter(transformComponent.m_Translate.x, transformComponent.m_Translate.y);
			view.setRotation(transformComponent.m_Rotate.z);
			view.setSize({ Screen::width, Screen::height });
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
				const Vector2f size = Vector2f(spriteAsset.m_RectangleSize);

				// #note: inverse scale because of SFML
				const float scaleX = spriteComponent.m_Size.x / size.x;
				const float scaleY = spriteComponent.m_Size.y / size.y * -1.f;

				sf::Sprite sprite;
				sprite.setOrigin(size * 0.5f);
				sprite.setPosition(transformComponent.m_Translate.x, transformComponent.m_Translate.y);
				sprite.setRotation(transformComponent.m_Rotate.z);
				sprite.setScale(transformComponent.m_Scale.x * scaleX, transformComponent.m_Scale.y * scaleY);
				sprite.setTexture(texture);
				sprite.setTextureRect(sf::IntRect
				(
					spriteAsset.m_RectanglePos.x
					, spriteAsset.m_RectanglePos.y
					, spriteAsset.m_RectangleSize.x
					, spriteAsset.m_RectangleSize.y
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

				if (!flipbookComponent.m_Flipbook)
					continue;

				const render::FlipbookAsset& flipbookAsset = flipbookComponent.m_Flipbook.get();
				if (flipbookAsset.m_Frames.empty())
					continue;

				const int32 frameMax = static_cast<int32>(flipbookAsset.m_Frames.size()) - 1;
				const int32 frameIndex = std::clamp(flipbookComponent.m_Index, 0, frameMax);

				const render::FlipbookFrame& flipbookFrame = flipbookAsset.m_Frames[frameIndex];
				if (!flipbookFrame.m_Sprite)
					continue;

				const render::SpriteAsset& spriteAsset = flipbookFrame.m_Sprite.get();
				if (!spriteAsset.m_Texture)
					continue;

				const render::TextureAsset& textureAsset = spriteAsset.m_Texture.get();

				const sf::Texture& texture = textureAsset.m_Texture;
				const Vector2f size = Vector2f(spriteAsset.m_RectangleSize);

				const float scaleX = flipbookComponent.m_Size.x / size.x;
				const float scaleY = flipbookComponent.m_Size.y / size.y * -1.f;

				sf::Sprite sprite;
				sprite.setOrigin(size * 0.5f);
				sprite.setPosition(transformComponent.m_Translate.x, transformComponent.m_Translate.y);
				sprite.setRotation(transformComponent.m_Rotate.z);
				sprite.setScale(transformComponent.m_Scale.x * scaleX, transformComponent.m_Scale.y * scaleY);
				sprite.setTexture(texture);
				sprite.setTextureRect(sf::IntRect
				(
					spriteAsset.m_RectanglePos.x
					, spriteAsset.m_RectanglePos.y
					, spriteAsset.m_RectangleSize.x
					, spriteAsset.m_RectangleSize.y
				));

				m_RenderTarget.draw(sprite);
			}
		}
	}
}
