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

REFL_AUTO
(
	type(physics::RigidStaticComponent)
	, field(m_Actor)
	, field(m_Shapes)
)