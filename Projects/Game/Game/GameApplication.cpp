#include "Game/GameApplication.h"

#include "Game/Game.h"

#include <entt/entt.hpp>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics.hpp>

GameApplication::GameApplication()
{
	m_Game = new Game();
}

GameApplication::~GameApplication()
{
	delete m_Game;
}

bool GameApplication::Initialise(int argc, char* argv[])
{
	ImGui::SFML::Init(*m_Window);
	m_Game->Initialise(m_Registry);
	return true;
}

void GameApplication::Destroy()
{
	ImGui::SFML::Shutdown();
	m_Game->Destroy(m_Registry);
}

bool GameApplication::Update()
{
	if (!m_Window->isOpen())
		return false;

	sf::Time time = m_Clock->restart();

	sf::Event event;
	while (m_Window->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			m_Window->close();
			return false;
		};
	}

	ImGui::SFML::Update(*m_Window, time);
	m_Game->Update(m_Registry, time);

	return true;
}

bool GameApplication::Render()
{
	//ImGui::ShowDemoWindow();

	m_Window->clear();
	m_Game->Render(m_Registry, m_Window);
	ImGui::SFML::Render(*m_Window);
	m_Window->display();

	return true;
}