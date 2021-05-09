#include "EditorPCH.h"
#include "Editor/Application.h"

#include "Editor/AssetBrowser.h"
#include "Editor/AssetPopup.h"
#include "Editor/EntityBrowser.h"
#include "Editor/ExampleComponent.h"
#include "Editor/FlipbookEditor.h"
#include "Editor/Historian.h"
#include "Editor/Inspector.h"
#include "Editor/MainMenuBar.h"
#include "Editor/SceneEditor.h"
#include "Editor/SpriteEditor.h"
#include "Editor/SpriteExtractor.h"

#include <Engine/AssetManager.h>
#include <Engine/CameraComponent.h>
#include <Engine/EntityWorld.h>
#include <Engine/FlipbookAsset.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/LevelSystem.h>
#include <Engine/NameComponent.h>
#include <Engine/PhysicsSystem.h>
#include <Engine/Screen.h>
#include <Engine/SoundSystem.h>
#include <Engine/SpriteComponent.h>
#include <Engine/TextureAsset.h>
#include <Engine/TransformComponent.h>

#include <imgui/imgui_internal.h>
#include <imgui/Helpers.h>
#include <imgui-sfml/imgui-SFML.h>
#include <ImGuizmo/ImGuizmo.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace
{
	const str::Name strDefaultFlipbook = NAME("9cf3dbdc-769e-a09a-6e4a-a0390e246666");
	const str::Name strDefaultMaterial = NAME("a4835493-ae5a-40ba-8083-06deb381c801");
	const str::Name strDefaultSprite = NAME("52ffdca6-bc1d-6423-0eda-0e2056e9662b");
	const str::Name strDefaultSound = NAME("18fa78b3-6f37-4680-81b4-951747f76f3a");
	const str::Name strDefaultTexture = NAME("5ed3b860-d64c-459a-a188-80dc7cc0e85f");
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

	m_EntityWorld.RegisterComponent<example::ExampleComponent>();

	m_EntityWorld.RegisterSystem<editor::Historian>();
	m_EntityWorld.RegisterSystem<editor::FlipbookEditor>();
	m_EntityWorld.RegisterSystem<editor::SceneEditor>(m_RenderTexture);
	m_EntityWorld.RegisterSystem<editor::SpriteEditor>();
	m_EntityWorld.RegisterSystem<editor::SpriteExtractor>();
	m_EntityWorld.RegisterSystem<editor::AssetBrowser>
		(
			m_AssetManager
			, m_EntityWorld.GetSystem<editor::FlipbookEditor>()
			, m_EntityWorld.GetSystem<editor::SpriteEditor>()
			, m_EntityWorld.GetSystem<editor::SpriteExtractor>()
		);
	m_EntityWorld.RegisterSystem<editor::Inspector>
		(
			m_EntityWorld.GetSystem<editor::Historian>()
		);
	m_EntityWorld.RegisterSystem<editor::EntityBrowser>
		(
			m_EntityWorld.GetSystem<editor::Inspector>()
		);
	m_EntityWorld.RegisterSystem<editor::MainMenuBar>
		(
			m_EntityWorld.GetSystem<audio::SoundSystem>()
			, m_EntityWorld.GetSystem<editor::AssetBrowser>()
			, m_EntityWorld.GetSystem<editor::EntityBrowser>()
			, m_EntityWorld.GetSystem<editor::Historian>()
			, m_EntityWorld.GetSystem<editor::Inspector>()
		);
}

void editor::Application::Initialise()
{
	core::Application::Initialise();

	physics::PhysicsSystem& physicsSystem = m_EntityWorld.GetSystem<physics::PhysicsSystem>();
	entt::sink(physicsSystem.m_OnCollideSignal).connect<&editor::Application::PlaySound>(this);

	core::LevelSystem& levelSystem = m_EntityWorld.GetSystem<core::LevelSystem>();
	levelSystem.Load(str::Path("Assets/Levels/Default/"));

	// example entity
	{
		entt::entity entity = m_EntityWorld.m_Registry.create();
		auto& exampleComponent = m_EntityWorld.m_Registry.emplace<example::ExampleComponent>(entity);
		//exampleComponent.m_PhysicsMaterial = m_AssetManager->LoadAsset<physics::MaterialAsset>(strDefaultMaterial);
		//exampleComponent.m_Sound = m_AssetManager->LoadAsset<audio::SoundAsset>(strDefaultSound);
		//exampleComponent.m_Texture = m_AssetManager->LoadAsset<render::TextureAsset>(strDefaultTexture);
		auto& nameComponent = m_EntityWorld.m_Registry.emplace<core::NameComponent>(entity);
		nameComponent.m_Name = "Example";
	}

	// flipbook entity
	{
		entt::entity entity = m_EntityWorld.m_Registry.create();
		auto& transformComponent = m_EntityWorld.m_Registry.emplace<core::TransformComponent>(entity);
		auto& flipbookComponent = m_EntityWorld.m_Registry.emplace<render::FlipbookComponent>(entity);
		flipbookComponent.m_Flipbook = m_AssetManager.LoadAsset<render::FlipbookAsset>(strDefaultFlipbook);
		flipbookComponent.m_Size = { 100, 100 };
		auto& nameComponent = m_EntityWorld.m_Registry.emplace<core::NameComponent>(entity);
		nameComponent.m_Name = "Flipbook";
	}

	// sprite entity
	{
		entt::entity entity = m_EntityWorld.m_Registry.create();
		auto& transformComponent = m_EntityWorld.m_Registry.emplace<core::TransformComponent>(entity);
		auto& spriteComponent = m_EntityWorld.m_Registry.emplace<render::SpriteComponent>(entity);
		spriteComponent.m_Sprite = m_AssetManager.LoadAsset<render::SpriteAsset>(strDefaultSprite);
		spriteComponent.m_Size = { 100, 100 };
		auto& nameComponent = m_EntityWorld.m_Registry.emplace<core::NameComponent>(entity);
		nameComponent.m_Name = "Sprite";
	}
}

void editor::Application::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ImGuizmo::BeginFrame();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	core::Application::Update(gameTime);
}

void editor::Application::Destroy()
{
	return core::Application::Destroy();
}

void editor::Application::PlaySound()
{
	m_EntityWorld.GetSystem<audio::SoundSystem>().PlaySound(strDefaultSound);
}
