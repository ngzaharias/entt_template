#pragma once

#include <vector>

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include <entt/signal/sigh.hpp>
#include <PhysX/PxSimulationEventCallback.h>

namespace physx
{
	class PxMaterial;
	class PxPhysics;
	class PxScene;

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
	class PhysicsSystem final : public physx::PxSimulationEventCallback
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Initialize(entt::registry& registry);
		void Destroy(entt::registry& registry);

		void Update(entt::registry& registry, const sf::Time& time);

	private:
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override { }
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void onWake(physx::PxActor** actors, physx::PxU32 count) override;

		void OnDestroy_RigidBody(entt::registry& registry, entt::entity entity);

	public:
		entt::sigh<void(const entt::entity&, const entt::entity&)> m_OnContactSignal;
		entt::sigh<void(const entt::entity&, const entt::entity&)> m_OnTriggerSignal;

	public:
		physx::PxMaterial* m_Material = nullptr;
		physx::PxPhysics* m_Physics = nullptr;
		physx::PxScene* m_Scene = nullptr;

		// debug
		physx::PxPvd* m_Debugger = nullptr;
		physx::PxPvdTransport* m_Transport = nullptr;

		float m_DeltaTimeAccumulated = 0.f;
	};
};