#include "Engine/EnginePCH.h"
#include "Engine/Application.h"

#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/EnttDebugger.h"
#include "Engine/FileHelpers.h"
#include "Engine/FlipbookSystem.h"
#include "Engine/LevelComponent.h"
#include "Engine/LevelSystem.h"
#include "Engine/NameComponent.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PhysicsSystem.h"
#include "Engine/RenderSystem.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/Screen.h"
#include "Engine/SoundSystem.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"

#include <random>
#include <time.h>
#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System.hpp>

#include <fstream>

core::Application::Application() 
{ 
	m_Clock = new sf::Clock();

	const uint32 width = static_cast<unsigned int>(Screen::width);
	const uint32 height = static_cast<unsigned int>(Screen::height);

	const sf::VideoMode videoMode = sf::VideoMode(width, height);
	const sf::Uint32 style = sf::Style::Default;
	sf::ContextSettings settings;
	settings.majorVersion = 0;
	settings.minorVersion = 1;

	m_RenderWindow = new sf::RenderWindow(videoMode, "...", style, settings);
	m_RenderTarget = m_RenderWindow;

	srand((unsigned int)time(NULL));
}

core::Application::~Application()
{ 
	delete m_Clock;
	delete m_RenderWindow;
}

void core::Application::Execute(int argc, char* argv[])
{
	ImGui::SFML::Init(*m_RenderWindow);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	Register();
	if (!Initialise())
		return;

	while (true)
	{
		if (!m_RenderWindow->isOpen())
			break;

		sf::Time time = m_Clock->restart();

		sf::Event event;
		while (m_RenderWindow->pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			switch (event.type)
			{
			case sf::Event::Closed:
				m_RenderWindow->close();
				break;
			};
		}

		// #todo: move into render system?
		m_RenderTarget->clear();
		m_RenderWindow->clear();

		ImGui::SFML::Update(*m_RenderWindow, time);

		if (!Update(time))
			break;

		// #todo: move into render system?
		//ImGui::ShowDemoWindow();

		ImGui::SFML::Render(*m_RenderWindow);

		m_RenderWindow->display();

	}

	Destroy();

	ImGui::SFML::Shutdown();
}

void core::Application::Register()
{
	// managers
	m_PhysicsManager = new physics::PhysicsManager();
	m_AssetManager = new core::AssetManager(*m_PhysicsManager);

	// components
	RegisterComponent<core::CameraComponent>();
	RegisterComponent<core::LevelComponent>();
	RegisterComponent<debug::NameComponent>();
	RegisterComponent<physics::RigidDynamicComponent>();
	RegisterComponent<physics::RigidStaticComponent>();
	RegisterComponent<render::SpriteComponent>();
	RegisterComponent<core::TransformComponent>();

	// systems
	RegisterSystem<render::FlipbookSystem>();
	RegisterSystem<render::RenderSystem>(*m_RenderTarget);
	RegisterSystem<physics::PhysicsSystem>(*m_PhysicsManager);
	RegisterSystem<audio::SoundSystem>(*m_AssetManager);
	RegisterSystem<core::LevelSystem>
		(
			*m_PhysicsManager 
			, *m_AssetManager
		);
	RegisterSystem<debug::EnttDebugger>();
}

bool core::Application::Initialise()
{
	// managers
	m_PhysicsManager->Initialize();
	m_AssetManager->Initialize();

	// systems
	for (core::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Initialize(m_Registry);
	}

	return true;
}

bool core::Application::Update(const core::GameTime& gameTime)
{
	// systems
	for (core::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Update(m_Registry, gameTime);
	}

	return true;
}

void core::Application::Destroy()
{
	// components
	m_ComponentEntries.clear();

	// systems
	for (core::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Destroy(m_Registry);
		delete entry.m_System;
	}
	m_SystemEntries.clear();

	// managers
	m_PhysicsManager->Destroy();
	m_AssetManager->Destroy();
	delete m_PhysicsManager;
	delete m_AssetManager;
}
