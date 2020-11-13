#include "Game/Application.h"

game::Application::Application()
{
}

game::Application::~Application()
{
}

void game::Application::Register()
{
	core::Application::Register();
}

bool game::Application::Initialise()
{
	core::Application::Initialise();

	return true;
}

bool game::Application::Update(const sf::Time& time)
{
	if (!core::Application::Update(time))
		return false;

	return true;
}

void game::Application::Destroy()
{
	return core::Application::Destroy();
}
