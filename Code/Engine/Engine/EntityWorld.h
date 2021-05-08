#pragma once

#include <Engine/GameTime.h>

#include <entt/entity/registry.hpp>

namespace ecs
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
		System* m_System;
	};

	class EntityWorld final
	{
	public:
		void Initialise();
		void Update(const core::GameTime& gameTime);
		void Destroy();

		template<class TSystem>
		TSystem& GetSystem() const;

		template<class TComponent>
		void RegisterComponent();

		template<class TSystem, typename... TArgs>
		void RegisterSystem(TArgs&&... args);

	public:
		// entities
		entt::registry m_Registry;

	private:
		// components
		Array<ComponentEntry> m_ComponentEntries;
		// systems
		Array<SystemEntry> m_SystemEntries;
	};
}

#include "EntityWorld.inl"