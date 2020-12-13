#include "Engine/EnginePCH.h"
#include "Engine/Screen.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

float Screen::width = 1920.f;
float Screen::height = 1080.f;
sf::RenderWindow* Screen::s_window = nullptr;

sf::RenderWindow* Screen::GetWindow() 
{ 
	return s_window; 
}

void Screen::SetWindow(sf::RenderWindow* window) 
{ 
	s_window = window; 
}