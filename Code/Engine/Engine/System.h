#pragma once

#include <Engine/GameTime.h>
#include <Engine/World.h>

namespace ecs
{
	class System
	{
		friend class ecs::World;

	public:
		virtual void Initialise() { }
		virtual void Destroy() { }

		virtual void Update(const core::GameTime& gameTime) { }

	protected:
		ecs::World* m_World = nullptr;
	};
}