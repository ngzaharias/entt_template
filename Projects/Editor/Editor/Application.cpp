#include "Editor/Application.h"

#include "Editor/AssetBrowser.h"
#include "Editor/Inspector.h"
#include "Editor/MainMenuBar.h"

editor::Application::Application()
{
}

editor::Application::~Application()
{
}

void editor::Application::Register()
{
	core::Application::Register();

	RegisterSystem<editor::AssetBrowser>(*m_ResourceManager);
	RegisterSystem<editor::Inspector>(*m_ResourceManager);
	RegisterSystem<editor::MainMenuBar>
		(
			GetSystem<editor::AssetBrowser>()
			, GetSystem<editor::Inspector>()
		);
}

bool editor::Application::Initialise()
{
	core::Application::Initialise();

	return true;
}

bool editor::Application::Update(const sf::Time& time)
{
	if (!core::Application::Update(time))
		return false;

	return true;
}

void editor::Application::Destroy()
{
	return core::Application::Destroy();
}
