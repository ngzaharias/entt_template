#include "Engine/Application.h"

#include "Engine/EnttDebugger.h"
#include "Engine/LevelSystem.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PhysicsSystem.h"
#include "Engine/RenderSystem.h"
#include "Engine/ResourceManager.h"
#include "Engine/Screen.h"
#include "Engine/SoundSystem.h"

#include <random>
#include <time.h>
#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

core::Application::Application() 
{ 
	const unsigned int width = static_cast<unsigned int>(Screen::width);
	const unsigned int height = static_cast<unsigned int>(Screen::height);

	const sf::VideoMode videoMode = sf::VideoMode(width, height);
	m_Window = new sf::RenderWindow(videoMode, "Breakout");
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
		//ImGui::ShowDemoWindow();
		ImGui::SFML::Render(*m_Window);
		// #todo: move into render system?
		m_Window->display();
	}

	Destroy();

	ImGui::SFML::Shutdown();
}

void core::Application::Register()
{
	// managers
	m_PhysicsManager = new physics::PhysicsManager();
	m_ResourceManager = new core::ResourceManager(*m_PhysicsManager);

	// systems
	RegisterSystem<render::RenderSystem>(*m_Window);
	RegisterSystem<physics::PhysicsSystem>(*m_PhysicsManager);
	RegisterSystem<audio::SoundSystem>(*m_ResourceManager);
	RegisterSystem<core::LevelSystem>
		(
			*m_PhysicsManager 
			, *m_ResourceManager
		);
	RegisterSystem<debug::EnttDebugger>();
}

bool core::Application::Initialise()
{
	// managers
	m_PhysicsManager->Initialize();
	m_ResourceManager->Initialize();

	// systems
	for (core::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Initialize(m_Registry);
	}

	return true;
}

bool core::Application::Update(const sf::Time& time)
{
	// systems
	for (core::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Update(m_Registry, time);
	}

	return true;
}

void core::Application::Destroy()
{
	// systems
	for (core::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Destroy(m_Registry);
		delete entry.m_System;
	}

	// managers
	m_PhysicsManager->Destroy();
	m_ResourceManager->Destroy();
	delete m_PhysicsManager;
	delete m_ResourceManager;
}
