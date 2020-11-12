#include "Engine/Application.h"

#include "Engine/Random.h"
#include "Engine/Screen.h"

#include <random>
#include <time.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Application::Application() 
{ 
	const unsigned int width = static_cast<unsigned int>(Screen::width);
	const unsigned int height = static_cast<unsigned int>(Screen::height);

	const sf::VideoMode videoMode = sf::VideoMode(width, height);
	m_Window = new sf::RenderWindow(videoMode, "Breakout");
	Screen::SetWindow(m_Window);

	m_Clock = new sf::Clock();

	srand((unsigned int)time(NULL));
}

Application::~Application() 
{ 
	delete m_Clock;
	delete m_Window;

	Screen::SetWindow(nullptr);
}

void Application::Execute(int argc, char* argv[])
{
	if (!Initialise(argc, argv))
		return;

	while (true)
	{
		if (!Update())
			break;

		if (!Render())
			break;
	}
	Destroy();
}
