#pragma once

#include <Engine/EntityWorld.h>
#include <Engine/GameTime.h>

namespace ecs
{
	class System
	{
		friend class EntityWorld;

	public:
		virtual void Initialise() { }
		virtual void Destroy() { }

		virtual void Update(const core::GameTime& gameTime) { }

	protected:
		EntityWorld* m_World = nullptr;
	};
}