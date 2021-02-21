#pragma once

#include <vector>

namespace physx
{
	class PxRigidDynamic;
	class PxShape;
}

namespace physics
{
	struct RigidDynamicComponent
	{
		physx::PxRigidDynamic* m_Actor;
		std::vector<physx::PxShape*> m_Shapes;
	};
}

REFL_AUTO
(
	type(physics::RigidDynamicComponent)
	, field(m_Actor)
	, field(m_Shapes)
)