#include "GamePCH.h"
#include "Game/Application.h"

#include <Engine/LevelSystem.h>
#include <Engine/Name.h>
#include <Engine/PhysicsSystem.h>
#include <Engine/Screen.h>
#include <Engine/SoundSystem.h>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

namespace
{
	const str::Path strDefaultLevel = str::Path("Assets/Levels/Default/");
	const str::Guid strDefaultSound = GUID("18fa78b3-6f37-4680-81b4-951747f76f3a");
}

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

void game::Application::Initialise()
{
	core::Application::Initialise();

	physics::PhysicsSystem& physicsSystem = m_World.GetSystem<physics::PhysicsSystem>();
	entt::sink(physicsSystem.m_OnCollideSignal).connect<&game::Application::PlaySound>(this);

	core::LevelSystem& levelSystem = m_World.GetSystem<core::LevelSystem>();
	levelSystem.Load(strDefaultLevel);
}

void game::Application::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	core::Application::Update(gameTime);
}

void game::Application::Destroy()
{
	return core::Application::Destroy();
}

void game::Application::PlaySound()
{
	m_World.GetSystem<audio::SoundSystem>().PlaySound(strDefaultSound);
}
