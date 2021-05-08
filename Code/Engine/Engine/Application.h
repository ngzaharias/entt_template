#pragma once

#include <Engine/AssetManager.h>
#include <Engine/EntityWorld.h>
#include <Engine/GameTime.h>
#include <Engine/PhysicsManager.h>
#include <Engine/TypeList.h>

#include <entt/core/type_info.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

namespace sf
{
	class Clock;
}

namespace core
{
	class Application
	{
	public:
		Application();
		~Application();

		void Execute(int argc, char* argv[]);

	protected:
		virtual void Register();
		virtual void Initialise();
		virtual void Update(const core::GameTime& gameTime);
		virtual void Destroy();

	protected:
		// managers
		core::AssetManager m_AssetManager;
		physics::PhysicsManager m_PhysicsManager;

		// ecs
		ecs::EntityWorld m_EntityWorld;

		sf::Clock m_Clock;
		sf::RenderTexture m_RenderTexture;
		sf::RenderWindow m_RenderWindow;
	};
}