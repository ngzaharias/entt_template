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

	// test entity
	{
		entt::entity entity = m_Registry.create();
		auto& component = m_Registry.emplace<example::Component>(entity);
		component.m_PhysicsMaterial = m_AssetManager->LoadAsset<physics::MaterialAsset>(strDefaultMaterial);
		component.m_Sound = m_AssetManager->LoadAsset<audio::SoundAsset>(strDefaultSound);
		component.m_Texture = m_AssetManager->LoadAsset<render::TextureAsset>(strDefaultTexture);

		editor::Inspector& inspector = GetSystem<editor::Inspector>();
		inspector.SetEntity(entity);
	}

	return true;
}

bool editor::Application::Update(const core::GameTime& gameTime)
{
	if (!core::Application::Update(gameTime))
		return false;

	return true;
}

void editor::Application::Destroy()
{
	return core::Application::Destroy();
}
