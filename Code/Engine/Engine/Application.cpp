#include "EnginePCH.h"
#include "Engine/Application.h"

#include "Engine/CameraComponent.h"
#include "Engine/DebugSystem.h"
#include "Engine/FileHelpers.h"
#include "Engine/FlipbookSystem.h"
#include "Engine/InputComponent.h"
#include "Engine/LevelComponent.h"
#include "Engine/LevelSystem.h"
#include "Engine/NameComponent.h"
#include "Engine/PhysicsSystem.h"
#include "Engine/RenderSystem.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/Screen.h"
#include "Engine/SoundSystem.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"

#include <time.h>
#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <random>
#include <set>

core::Application::Application()
	: m_AssetManager(m_PhysicsManager)
	, m_PhysicsManager()
{ 
	const uint32 width = static_cast<unsigned int>(Screen::width);
	const uint32 height = static_cast<unsigned int>(Screen::height);

	sf::ContextSettings settings;
	settings.majorVersion = 0;
	settings.minorVersion = 1;

	const sf::Uint32 style = sf::Style::Default;
	const sf::VideoMode videoMode = sf::VideoMode(width, height);
	m_RenderTexture.create(width, height, settings);
	m_RenderWindow.create(videoMode, "...", style, settings);

	srand((unsigned int)time(NULL));
}

core::Application::~Application()
{ 
}

void core::Application::Execute(int argc, char* argv[])
{
	ImGui::SFML::Init(m_RenderWindow);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	Register();
	Initialise();

	while (true)
	{
		PROFILE_TICK("MainThread");

		m_RenderTexture.clear();
		if (!m_RenderWindow.isOpen())
			break;

		sf::Time time = m_Clock.restart();

		sf::Event eventData;
		while (m_RenderWindow.pollEvent(eventData))
		{
			ImGui::SFML::ProcessEvent(eventData);

			switch (eventData.type)
			{
			case sf::Event::Closed:
				m_RenderWindow.close();
				break;
			};
		}

		Set<sf::Keyboard::Key> keysPressed;
		for (int32 i = sf::Keyboard::Key::A; i < sf::Keyboard::Key::KeyCount; ++i)
		{
			const sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(i);
			if (sf::Keyboard::isKeyPressed(key))
				keysPressed.insert(key);
		}

		for (auto& entity : m_Registry.view<input::InputComponent>())
		{
			auto& component = m_Registry.get<input::InputComponent>(entity);
			component.m_KeysPrevious = component.m_KeysCurrent;
			component.m_KeysCurrent = keysPressed;
		}

		ImGui::SFML::Update(m_RenderWindow, time);
		Update(time);

		// render to window
		{
			const sf::Texture& texture = m_RenderTexture.getTexture();
			const Vector2f size = Vector2f(texture.getSize());

			sf::Sprite sprite;
			sprite.setTexture(texture);
			m_RenderWindow.draw(sprite);
		}
		ImGui::SFML::Render(m_RenderWindow);

		// #todo
		m_RenderWindow.display();
	}

	Destroy();

	ImGui::SFML::Shutdown();
}

void core::Application::Register()
{
	// components
	RegisterComponent<core::CameraComponent>();
	RegisterComponent<core::LevelComponent>();
	RegisterComponent<core::NameComponent>();
	RegisterComponent<core::TransformComponent>();
	RegisterComponent<input::InputComponent>();
	RegisterComponent<physics::RigidDynamicComponent>();
	RegisterComponent<physics::RigidStaticComponent>();
	RegisterComponent<render::SpriteComponent>();

	// systems
	RegisterSystem<render::FlipbookSystem>();
	RegisterSystem<render::RenderSystem>(m_RenderTexture);
	RegisterSystem<physics::PhysicsSystem>(m_PhysicsManager);
	RegisterSystem<audio::SoundSystem>(m_AssetManager);
	RegisterSystem<core::LevelSystem>
		(
			m_PhysicsManager 
			, m_AssetManager
		);

	// register last
	RegisterSystem<debug::DebugSystem>(m_RenderTexture);
}

void core::Application::Initialise()
{
	{
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<core::NameComponent>(entity).m_Name = "Input";
		m_Registry.emplace<input::InputComponent>(entity);
	}

	// managers
	m_AssetManager.Initialize();
	m_PhysicsManager.Initialize();

	// systems
	for (core::SystemEntry& entry : m_SystemEntries)
		entry.m_System->Initialize(m_Registry);

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
}

void core::Application::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// systems
	for (core::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Update(m_Registry, gameTime);
	}
}

void core::Application::Destroy()
{
	// ecs
	m_ComponentEntries.clear();
	for (core::SystemEntry& entry : m_SystemEntries)
	{
		entry.m_System->Destroy(m_Registry);
		delete entry.m_System;
	}
	m_SystemEntries.clear();

	// managers
	m_AssetManager.Destroy();
	m_PhysicsManager.Destroy();
}
