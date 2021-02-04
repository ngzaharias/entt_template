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
		if (m_RenderTarget != m_RenderWindow)
			m_RenderTarget->clear();
		m_RenderWindow->clear();

		ImGui::SFML::Update(*m_RenderWindow, time);

		if (!Update(time))
			break;

		ImGui::SFML::Render(*m_RenderWindow);

		// #todo
		//m_RenderTexture->display();
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
	RegisterComponent<core::NameComponent>();
	RegisterComponent<core::TransformComponent>();
	RegisterComponent<physics::RigidDynamicComponent>();
	RegisterComponent<physics::RigidStaticComponent>();
	RegisterComponent<render::SpriteComponent>();

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


	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	//ImGuiStyle& style = ImGui::GetStyle();
	//style.Colors[ImGuiCol_Header] = s_PurpleEnabled;
	//style.Colors[ImGuiCol_HeaderActive] = s_PurpleSelected;
	//style.Colors[ImGuiCol_HeaderHovered] = s_PurpleHovered;
	//style.Colors[ImGuiCol_ModalWindowDimBg] = s_Gray;
	//style.Colors[ImGuiCol_ResizeGrip] = s_PurpleEnabled;
	//style.Colors[ImGuiCol_ResizeGripActive] = s_PurpleSelected;
	//style.Colors[ImGuiCol_ResizeGripHovered] = s_PurpleHovered;
	//style.Colors[ImGuiCol_Separator] = s_PurpleHovered;
	//style.Colors[ImGuiCol_TitleBg] = s_PurpleEnabled;
	//style.Colors[ImGuiCol_TitleBgActive] = s_PurpleEnabled;
	//style.Colors[ImGuiCol_TitleBgCollapsed] = s_PurpleEnabled;

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
