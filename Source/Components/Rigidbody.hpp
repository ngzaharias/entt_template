#pragma once

#include <vector>

namespace physx
{
	class PxRigidActor;
	class PxShape;
}

namespace physics
{
	struct RigidBody
	{
		physx::PxRigidActor* m_Actor;
		std::vector<physx::PxShape*> m_Shapes;
	};
}