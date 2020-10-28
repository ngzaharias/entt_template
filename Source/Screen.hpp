#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
	class Window;
}

class Screen
{
public:
	static sf::RenderWindow* GetWindow();
	static void SetWindow(sf::RenderWindow* window);

public:
	static float width;
	static float height;

private:
	static sf::RenderWindow* s_window;

};
