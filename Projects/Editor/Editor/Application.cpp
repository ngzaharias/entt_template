#include "EditorPCH.h"
#include "Editor/Application.h"

#include "Editor/AssetBrowser.h"
#include "Editor/AssetPopup.h"
#include "Editor/EntityBrowser.h"
#include "Editor/FlipbookEditor.h"
#include "Editor/GameWindow.h"
#include "Editor/History.h"
#include "Editor/Inspector.h"
#include "Editor/InspectorExamples.h"
#include "Editor/MainMenuBar.h"
#include "Editor/SceneWindow.h"
#include "Editor/SpriteEditor.h"
#include "Editor/SpriteExtractor.h"

#include <Engine/AssetManager.h>
#include <Engine/CameraComponent.h>
#include <Engine/FlipbookAsset.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/Screen.h>
#include <Engine/SpriteComponent.h>
#include <Engine/TextureAsset.h>
#include <Engine/TransformComponent.h>

#include <imgui-sfml/imgui-SFML.h>
#include <ImGuizmo/ImGuizmo.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace
{
	const str::Name strDefaultFlipbook = str::Name::Create("9cf3dbdc-769e-a09a-6e4a-a0390e246666");
	const str::Name strDefaultMaterial = str::Name::Create("a4835493-ae5a-40ba-8083-06deb381c801");
	const str::Name strDefaultSprite = str::Name::Create("a55ac594-c274-372d-2f0b-733e79dfbabb");
	const str::Name strDefaultSound = str::Name::Create("18fa78b3-6f37-4680-81b4-951747f76f3a");
	const str::Name strDefaultTexture = str::Name::Create("5ed3b860-d64c-459a-a188-80dc7cc0e85f");
}

editor::Application::Application()
{
	const uint32 width = static_cast<unsigned int>(Screen::width);
	const uint32 height = static_cast<unsigned int>(Screen::height);

	m_RenderTexture = new sf::RenderTexture();
	m_RenderTexture->create(width, height);
	m_RenderTarget = m_RenderTexture;
}

editor::Application::~Application()
{
	delete m_RenderTexture;
}

void editor::Application::Register()
{
	core::Application::Register();

	RegisterComponent<example::Component>();

	RegisterSystem<editor::FlipbookEditor>();
	RegisterSystem<editor::GameWindow>(*m_RenderTexture);
	RegisterSystem<editor::SceneWindow>(*m_RenderTexture);
	RegisterSystem<editor::SpriteEditor>();
	RegisterSystem<editor::SpriteExtractor>();
	RegisterSystem<editor::AssetBrowser>
		(
			*m_AssetManager
			, GetSystem<editor::FlipbookEditor>()
			, GetSystem<editor::SpriteEditor>()
			, GetSystem<editor::SpriteExtractor>()
		);
	RegisterSystem<editor::Inspector>();
	RegisterSystem<editor::EntityBrowser>
		(
			GetSystem<editor::Inspector>()
		);
	RegisterSystem<editor::History>();
	RegisterSystem<editor::MainMenuBar>
		(
			GetSystem<editor::AssetBrowser>()
			, GetSystem<editor::EntityBrowser>()
			, GetSystem<editor::GameWindow>()
			, GetSystem<editor::History>()
			, GetSystem<editor::Inspector>()
		);
}

bool editor::Application::Initialise()
{
	core::Application::Initialise();

	// camera entity
	{
		entt::entity entity = m_Registry.create();
		auto& cameraComponent = m_Registry.emplace<core::CameraComponent>(entity);
		auto& transformComponent = m_Registry.emplace<core::TransformComponent>(entity);
		auto& nameComponent = m_Registry.emplace<core::NameComponent>(entity);
		nameComponent.m_Name = "Camera";
	}

	// example entity
	{
		entt::entity entity = m_Registry.create();
		auto& exampleComponent = m_Registry.emplace<example::Component>(entity);
		exampleComponent.m_PhysicsMaterial = m_AssetManager->LoadAsset<physics::MaterialAsset>(strDefaultMaterial);
		exampleComponent.m_Sound = m_AssetManager->LoadAsset<audio::SoundAsset>(strDefaultSound);
		exampleComponent.m_Texture = m_AssetManager->LoadAsset<render::TextureAsset>(strDefaultTexture);
		auto& nameComponent = m_Registry.emplace<core::NameComponent>(entity);
		nameComponent.m_Name = "Example";
	}

	// flipbook entity
	{
		entt::entity entity = m_Registry.create();
		auto& transformComponent = m_Registry.emplace<core::TransformComponent>(entity);
		auto& flipbookComponent = m_Registry.emplace<render::FlipbookComponent>(entity);
		flipbookComponent.m_Flipbook = m_AssetManager->LoadAsset<render::FlipbookAsset>(strDefaultFlipbook);
		flipbookComponent.m_Size = { 100, 100 };
		auto& nameComponent = m_Registry.emplace<core::NameComponent>(entity);
		nameComponent.m_Name = "Flipbook";
	}

	// sprite entity
	{
		entt::entity entity = m_Registry.create();
		auto& transformComponent = m_Registry.emplace<core::TransformComponent>(entity);
		auto& spriteComponent = m_Registry.emplace<render::SpriteComponent>(entity);
		spriteComponent.m_Sprite = m_AssetManager->LoadAsset<render::SpriteAsset>(strDefaultSprite);
		spriteComponent.m_Size = { 100, 100 };
		auto& nameComponent = m_Registry.emplace<core::NameComponent>(entity);
		nameComponent.m_Name = "Sprite";
	}

	return true;
}

bool editor::Application::Update(const core::GameTime& gameTime)
{
	ImGuizmo::BeginFrame();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (!core::Application::Update(gameTime))
		return false;

	return true;
}

void editor::Application::Destroy()
{
	return core::Application::Destroy();
}
