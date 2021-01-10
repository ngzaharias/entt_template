#include "Editor/EditorPCH.h"
#include "Editor/Application.h"

#include "Editor/AssetBrowser.h"
#include "Editor/AssetPopup.h"
#include "Editor/History.h"
#include "Editor/Inspector.h"
#include "Editor/InspectorExamples.h"
#include "Editor/MainMenuBar.h"

#include <Engine/AssetManager.h>
#include <Engine/CameraComponent.h>
#include <Engine/TextureAsset.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/SpriteComponent.h>
#include <Engine/TransformComponent.h>

namespace
{
	const str::Name strDefaultMaterial = str::Name::Create("a4835493-ae5a-40ba-8083-06deb381c801");
	const str::Name strDefaultSound = str::Name::Create("18fa78b3-6f37-4680-81b4-951747f76f3a");
	const str::Name strDefaultTexture = str::Name::Create("5ed3b860-d64c-459a-a188-80dc7cc0e85f");
}

editor::Application::Application()
{
}

editor::Application::~Application()
{
}

void editor::Application::Register()
{
	core::Application::Register();

	RegisterComponent<example::Component>();

	RegisterSystem<editor::AssetBrowser>(*m_AssetManager);
	RegisterSystem<editor::History>();
	RegisterSystem<editor::Inspector>();
	RegisterSystem<editor::MainMenuBar>
		(
			GetSystem<editor::AssetBrowser>()
			, GetSystem<editor::History>()
			, GetSystem<editor::Inspector>()
		);
}

bool editor::Application::Initialise()
{
	core::Application::Initialise();

	// camera
	{
		entt::entity entity = m_Registry.create();
		auto& transform = m_Registry.emplace<core::TransformComponent>(entity);
		auto& camera = m_Registry.emplace<core::CameraComponent>(entity);
		camera.m_Size = { 1280, 720 };
	}

	// test entity
	{
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<core::TransformComponent>(entity);
		auto& component = m_Registry.emplace<example::Component>(entity);

		component.m_PhysicsMaterial = m_AssetManager->LoadAsset<physics::MaterialAsset>(strDefaultMaterial);
		component.m_Sound = m_AssetManager->LoadAsset<audio::SoundAsset>(strDefaultSound);
		component.m_Texture = m_AssetManager->LoadAsset<render::TextureAsset>(strDefaultTexture);

		editor::Inspector& inspector = GetSystem<editor::Inspector>();
		inspector.SetEntity(entity);
	}

	auto CreateFlipbook = [&](const char* texture, const uint32 count)
	{
		const str::Name guid = str::Name::Create(texture);
		const render::TextureHandle handle = m_AssetManager->LoadAsset<render::TextureAsset>(guid);

		entt::entity entity = m_Registry.create();
		auto& transform = m_Registry.emplace<core::TransformComponent>(entity);
		auto& flipbook = m_Registry.emplace<render::FlipbookComponent>(entity);
		flipbook.m_FPS = 60.f;
		flipbook.m_Time = 0.f;
		flipbook.m_Index = 0;
		flipbook.m_SubSprite.m_Count = count;
		flipbook.m_SubSprite.m_Size = { 100, 100 };
		flipbook.m_Size = { 200.f, 200.f };
		flipbook.m_Sprite.setTexture(handle->m_Texture);
		flipbook.m_Sprite.setOrigin(sf::Vector2f(50.f, 50.f));
	};

	//CreateFlipbook("4bf7466a-f63d-6445-61c9-8b46e3958584", 61);	// MagicBubbles
	//CreateFlipbook("1e1881f1-c698-54ba-4ba1-395d6a539526", 61);	// ProtectionCircle
	CreateFlipbook("91ffda2b-d755-ce42-35c3-7010aaf5ec5a", 61);	// Midnight
	//CreateFlipbook("82111b6f-3b77-9230-d2a1-7c55abf95df3", 46);	// FlameLash
	//CreateFlipbook("5461128c-1405-12b0-1b0f-33023aa39208", 121);	// Loading
	//CreateFlipbook("f9dcfdd6-014a-c528-12e2-1e73f232b7f9", 61);		// Magic8

	return true;
}

bool editor::Application::Update(const sf::Time& time)
{
	if (!core::Application::Update(time))
		return false;

	return true;
}

void editor::Application::Destroy()
{
	return core::Application::Destroy();
}
