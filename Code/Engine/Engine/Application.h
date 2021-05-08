#pragma once

#include <Engine/AssetManager.h>
#include <Engine/PhysicsManager.h>
#include <Engine/System.h>
#include <Engine/TypeList.h>

#include <entt/fwd.hpp>
#include <entt/core/type_info.hpp>
#include <entt/entity/registry.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

namespace sf
{
	class Clock;
}

namespace core
{
	class System;

	struct ComponentEntry
	{
		using TypeId = entt::id_type;

		const TypeId m_TypeId = 0;
	};

	struct SystemEntry
	{
		using TypeId = entt::id_type;

		const TypeId m_TypeId = 0;
		core::System* m_System = nullptr;
	};

	class Application
	{
	public:
		Application();
		~Application();

		void Execute(int argc, char* argv[]);

		// #todo: move to an ECS class?
		template<class TSystem>
		TSystem& GetSystem() const;

		// #todo: move to an ECS class?
		template<class TComponent>
		void RegisterComponent();

		// #todo: move to an ECS class?
		template<class TSystem, typename... TArgs>
		void RegisterSystem(TArgs&&... args);

	protected:
		virtual void Register();
		virtual void Initialise();
		virtual void Update(const core::GameTime& gameTime);
		virtual void Destroy();

	protected:
		// managers
		core::AssetManager m_AssetManager;
		physics::PhysicsManager m_PhysicsManager;

		// entities
		entt::registry m_Registry;
		// components
		Array<ComponentEntry> m_ComponentEntries;
		// systems
		Array<SystemEntry> m_SystemEntries;

		sf::Clock m_Clock;
		sf::RenderTexture m_RenderTexture;
		sf::RenderWindow m_RenderWindow;
	};
}

#include "Application.inl"