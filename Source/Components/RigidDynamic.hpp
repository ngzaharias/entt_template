#pragma once

#include <vector>

namespace physx
{
	class PxRigidDynamic;
	class PxShape;
}

namespace physics
{
	struct RigidDynamic
	{
		physx::PxRigidDynamic* m_Actor;
		std::vector<physx::PxShape*> m_Shapes;
	};
}