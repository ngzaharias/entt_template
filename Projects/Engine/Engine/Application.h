#pragma once

#include <Engine/System.h>
#include <Engine/TypeList.h>

#include <entt/fwd.hpp>
#include <entt/core/type_info.hpp>
#include <entt/entity/registry.hpp>

namespace debug
{
	class EnttDebugger;
}

namespace physics
{
	class PhysicsManager;
}

namespace sf
{
	class Clock;
	class RenderWindow;
}

namespace core
{
	class AssetManager;
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
		virtual bool Initialise();
		virtual bool Update(const core::GameTime& gameTime);
		virtual void Destroy();

	protected:
		// managers
		physics::PhysicsManager* m_PhysicsManager = nullptr;
		core::AssetManager* m_AssetManager = nullptr;

		// components
		std::vector<ComponentEntry> m_ComponentEntries;

		// systems
		std::vector<SystemEntry> m_SystemEntries;

		entt::registry m_Registry;

		sf::Clock* m_Clock;
		sf::RenderWindow* m_Window;

		core::TypeList<> m_ComponentList;
	};
}

#include "Application.inl"