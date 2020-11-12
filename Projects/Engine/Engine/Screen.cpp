#include "Engine/Screen.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

float Screen::width = 1280;
float Screen::height = 720.f;
sf::RenderWindow* Screen::s_window = nullptr;

sf::RenderWindow* Screen::GetWindow() 
{ 
	return s_window; 
}

void Screen::SetWindow(sf::RenderWindow* window) 
{ 
	s_window = window; 
}