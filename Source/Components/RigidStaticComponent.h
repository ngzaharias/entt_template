#pragma once

#include <vector>

namespace physx
{
	class PxRigidStatic;
	class PxShape;
}

namespace physics
{
	struct RigidStaticComponent
	{
		physx::PxRigidStatic* m_Actor;
		std::vector<physx::PxShape*> m_Shapes;
	};
}