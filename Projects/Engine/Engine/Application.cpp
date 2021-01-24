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
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <fstream>

core::Application::Application() 
{ 
	const unsigned int width = static_cast<unsigned int>(Screen::width);
	const unsigned int height = static_cast<unsigned int>(Screen::height);

	const sf::VideoMode videoMode = sf::VideoMode(width, height);
	m_Window = new sf::RenderWindow(videoMode, "...");
	Screen::SetWindow(m_Window);

	m_Clock = new sf::Clock();

	srand((unsigned int)time(NULL));
}

core::Application::~Application()
{ 
	delete m_Clock;
	delete m_Window;

	Screen::SetWindow(nullptr);
}

void core::Application::Execute(int argc, char* argv[])
{
	ImGui::SFML::Init(*m_Window);

	Register();
	if (!Initialise())
		return;

	while (true)
	{
		if (!m_Window->isOpen())
			break;

		sf::Time time = m_Clock->restart();

		sf::Event event;
		while (m_Window->pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			switch (event.type)
			{
			case sf::Event::Closed:
				m_Window->close();
				break;
			};
		}

		// #todo: move into render system?
		m_Window->clear();

		ImGui::SFML::Update(*m_Window, time);

		if (!Update(time))
			break;

		// #todo: move into render system?
		ImGui::ShowDemoWindow();
		ImGui::SFML::Render(*m_Window);
		m_Window->display();
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
	RegisterSystem<render::RenderSystem>(*m_Window);
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
