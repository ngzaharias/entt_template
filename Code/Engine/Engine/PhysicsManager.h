#pragma once

#include <entt/signal/sigh.hpp>
#include <PhysX/PxSimulationEventCallback.h>

namespace physx
{
	class PxActor;
	class PxPhysics;
	class PxPvd;
	class PxPvdTransport;
	class PxRigidBody;
	class PxScene;
	class PxTransform;

	struct PxConstraintInfo;
	struct PxContactPair;
	struct PxContactPairHeader;
	struct PxTriggerPair;
}

namespace physics
{
	class PhysicsManager final : public physx::PxSimulationEventCallback
	{
	public:
		PhysicsManager();
		~PhysicsManager();

		void Initialise();
		void Destroy();

		physx::PxPhysics& GetPhysics() const { return *m_Physics; }
		physx::PxScene& GetScene() const { return *m_Scene; }

	private:
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override { }
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void onWake(physx::PxActor** actors, physx::PxU32 count) override;

	public:
		entt::sigh<void(const ecs::Entity&, const ecs::Entity&)> m_OnContactSignal;
		entt::sigh<void(const ecs::Entity&, const ecs::Entity&)> m_OnTriggerSignal;

	public:
		physx::PxPhysics* m_Physics = nullptr;
		physx::PxScene* m_Scene = nullptr;

		// debug
		physx::PxPvd* m_Debugger = nullptr;
		physx::PxPvdTransport* m_Transport = nullptr;

		int32 m_Int;
	};
}