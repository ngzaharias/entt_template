#pragma once

#include <entt/entt.hpp>

namespace sf
{
	class Clock;
	class RenderWindow;
}

class Application
{
public:
	Application();
	virtual ~Application();

	void Execute(int argc, char* argv[]);

protected:
	virtual bool Initialise(int argc, char* argv[]) = 0;
	virtual void Destroy() = 0;

	virtual bool Update() = 0;
	virtual bool Draw() = 0;

protected:
	entt::registry m_Registry;

	sf::Clock* m_Clock;
	sf::RenderWindow* m_Window;
};
