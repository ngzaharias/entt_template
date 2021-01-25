#include "Editor/EditorPCH.h"
#include "Editor/Application.h"

#include "Editor/AssetBrowser.h"
#include "Editor/AssetPopup.h"
#include "Editor/FlipbookEditor.h"
#include "Editor/History.h"
#include "Editor/Inspector.h"
#include "Editor/InspectorExamples.h"
#include "Editor/MainMenuBar.h"
#include "Editor/SpriteEditor.h"
#include "Editor/SpriteExtractor.h"

#include <Engine/AssetManager.h>
#include <Engine/CameraComponent.h>
#include <Engine/TextureAsset.h>
#include <Engine/FlipbookAsset.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/Screen.h>
#include <Engine/SpriteComponent.h>
#include <Engine/TransformComponent.h>

#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics/RenderTexture.hpp>

namespace
{
	const str::Name strDefaultFlipbook = str::Name::Create("9cf3dbdc-769e-a09a-6e4a-a0390e246666");
	const str::Name strDefaultMaterial = str::Name::Create("a4835493-ae5a-40ba-8083-06deb381c801");
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
	RegisterSystem<editor::SpriteEditor>();
	RegisterSystem<editor::SpriteExtractor>();
	RegisterSystem<editor::AssetBrowser>
		(
			*m_AssetManager
			, GetSystem<editor::FlipbookEditor>()
			, GetSystem<editor::SpriteEditor>()
			, GetSystem<editor::SpriteExtractor>()
		);
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

	{
		entt::entity entity = m_Registry.create();
		auto& cameraComponent = m_Registry.emplace<core::CameraComponent>(entity);
		cameraComponent.m_Size = { Screen::width, Screen::height };
		auto& transformComponent = m_Registry.emplace<core::TransformComponent>(entity);
	}

	// test entity
	{
		entt::entity entity = m_Registry.create();
		auto& exampleComponent = m_Registry.emplace<example::Component>(entity);
		exampleComponent.m_PhysicsMaterial = m_AssetManager->LoadAsset<physics::MaterialAsset>(strDefaultMaterial);
		exampleComponent.m_Sound = m_AssetManager->LoadAsset<audio::SoundAsset>(strDefaultSound);
		exampleComponent.m_Texture = m_AssetManager->LoadAsset<render::TextureAsset>(strDefaultTexture);

		auto& transformComponent = m_Registry.emplace<core::TransformComponent>(entity);

		auto& flipbookComponent = m_Registry.emplace<render::FlipbookComponent>(entity);
		flipbookComponent.m_Flipbook = m_AssetManager->LoadAsset<render::FlipbookAsset>(strDefaultFlipbook);

		editor::Inspector& inspector = GetSystem<editor::Inspector>();
		inspector.SetEntity(entity);
	}

	//refl::descriptor::type_descriptor reflected = refl::reflect<MyStruct>();
	//refl::descriptor::member_list<MyStruct> members = reflected.members;
	//for_each(refl::reflect<MyStruct>().members, [&](auto member)
	//{
	//	refl::descriptor::field_descriptor<MyStruct, 0>& field = member;
	//});

	return true;
}

bool editor::Application::Update(const core::GameTime& gameTime)
{
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (!core::Application::Update(gameTime))
		return false;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoScrollbar;
	flags |= ImGuiWindowFlags_NoScrollWithMouse;

	if (ImGui::Begin("Game", nullptr, flags))
	{
		ImGui::Image(m_RenderTexture->getTexture());
	}
	ImGui::End();

	return true;
}

void editor::Application::Destroy()
{
	return core::Application::Destroy();
}
