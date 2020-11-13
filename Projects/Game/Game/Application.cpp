#include "Game/Application.h"

#include <Engine/LevelSystem.h>
#include <Engine/PhysicsSystem.h>
#include <Engine/SoundSystem.h>

namespace
{
	const str::Path strDefaultPath = str::Path("Assets/Levels/Default/");
	const str::Path strExampleSound = str::Path("Assets/Sounds/S_Example.ogg");
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

bool game::Application::Initialise()
{
	core::Application::Initialise();

	physics::PhysicsSystem& physicsSystem = GetSystem<physics::PhysicsSystem>();
	entt::sink(physicsSystem.m_OnCollideSignal).connect<&game::Application::PlaySound>(this);

	core::LevelSystem& levelSystem = GetSystem<core::LevelSystem>();
	levelSystem.Load(m_Registry, strDefaultPath);

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

void game::Application::PlaySound()
{
	GetSystem<audio::SoundSystem>().PlaySound(strExampleSound);
}
