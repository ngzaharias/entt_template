#pragma once

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
		Array<physx::PxShape*> m_Shapes;
	};
}

REFL_AUTO
(
	type(physics::RigidStaticComponent)
	, field(m_Actor)
	, field(m_Shapes)
)