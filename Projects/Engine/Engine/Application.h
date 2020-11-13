#pragma once

#include <Engine/System.h>

#include <entt/fwd.hpp>
#include <entt/core/type_info.hpp>
#include <entt/entity/registry.hpp>

namespace debug
{
	class EnttDebugger;
}

namespace sf
{
	class Clock;
	class RenderWindow;
}

namespace core
{
	class ResourceManager;
	class System;

	struct SystemEntry
	{
		using TypeId = entt::id_type;

		core::System* m_System = nullptr;
		const TypeId m_TypeId;
	};

	class Application
	{
	public:
		Application();
		~Application();

		void Execute(int argc, char* argv[]);

		// #todo: move to a Registry class?
		template<class TSystem>
		TSystem* GetSystem();

		// #todo: move to a Registry class?
		template<class TSystem, typename... TArgs>
		void RegisterSystem(TArgs&&... args);

	protected:
		virtual void Register();
		virtual bool Initialise();
		virtual bool Update(const sf::Time& time);
		virtual void Destroy();

	protected:
		// managers
		core::ResourceManager* m_ResourceManager;

		// systems
		std::vector<SystemEntry> m_SystemEntries;

		entt::registry m_Registry;

		sf::Clock* m_Clock;
		sf::RenderWindow* m_Window;
	};
}

#include "Application.inl"