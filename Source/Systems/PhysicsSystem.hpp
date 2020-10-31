#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include <entt/signal/sigh.hpp>

namespace physx
{
	class PxMaterial;
	class PxPhysics;
	class PxRigidDynamic;
	class PxScene;
	class PxShape;

	// debug
	class PxPvd;
	class PxPvdTransport;
}

namespace sf
{
	class Time;
}

namespace physics
{
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Initialize(entt::registry& registry);
		void Destroy(entt::registry& registry);

		void Update(entt::registry& registry, const sf::Time& time);

	private:
		void OnDestroy_RigidBody(entt::registry& registry, entt::entity entity);

	public:
		entt::sigh<void(const entt::entity&, const entt::entity&)> m_OnCollisionSignal;

	private:
		physx::PxMaterial* m_Material;
		physx::PxPhysics* m_Physics;
		physx::PxScene* m_Scene;

		// debug
		physx::PxPvd* m_Debugger;
		physx::PxPvdTransport* m_Transport;
	};
};