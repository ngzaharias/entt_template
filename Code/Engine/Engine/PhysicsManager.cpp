#include "EnginePCH.h"
#include "Engine/PhysicsManager.h"

#include <PhysX/PxPhysicsAPI.h>

namespace physx
{
	class UserAllocatorCallback final : public physx::PxAllocatorCallback
	{
	public:
		void* allocate(size_t size, const char*, const char*, int) override
		{
			void* ptr = platformAlignedAlloc(size);
			PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
			return ptr;
		}

		void deallocate(void* ptr) override
		{
			platformAlignedFree(ptr);
		}
	};

	class UserErrorCallback final : public physx::PxErrorCallback
	{
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
		{
			printf("PhysX Error: %d %s %s %d", code, message, file, line);
		}
	};

	static UserAllocatorCallback gAllocator;
	static UserErrorCallback gError;
	static PxFoundation* gFoundation = nullptr;
	static PxDefaultCpuDispatcher* gDispatcher = nullptr;

	PxFilterFlags contactReportFilterShader
	(
		PxFilterObjectAttributes attributes0
		, PxFilterData filterData0
		, PxFilterObjectAttributes attributes1
		, PxFilterData filterData1
		, PxPairFlags& pairFlags
		, const void* constantBlock
		, PxU32 constantBlockSize
	)
	{
		PX_UNUSED(attributes0);
		PX_UNUSED(attributes1);
		PX_UNUSED(filterData0);
		PX_UNUSED(filterData1);
		PX_UNUSED(constantBlockSize);
		PX_UNUSED(constantBlock);

		// ignore kinematic <-> kinematic
		const bool isKinematic0 = physx::PxFilterObjectIsKinematic(attributes0);
		const bool isKinematic1 = physx::PxFilterObjectIsKinematic(attributes1);
		if (isKinematic0 && isKinematic1)
			return PxFilterFlag::eSUPPRESS;

		// ignore kinematic <-> rigid static
		const bool isRigidStatic0 = physx::PxGetFilterObjectType(attributes0) == PxFilterObjectType::eRIGID_STATIC;
		const bool isRigidStatic1 = physx::PxGetFilterObjectType(attributes1) == PxFilterObjectType::eRIGID_STATIC;
		if ((isKinematic0 && isRigidStatic1) || (isKinematic1 && isRigidStatic0))
			return PxFilterFlag::eSUPPRESS;

		const bool hasCollisionWord0 = filterData0.word0 & filterData1.word0;
		const bool hasCollisionWord1 = filterData0.word1 & filterData1.word1;
		const bool hasCollisionWord2 = filterData0.word2 & filterData1.word2;
		const bool hasCollisionWord3 = filterData0.word3 & filterData1.word3;

		const bool isBlocking = hasCollisionWord0;
		if (!isBlocking)
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eDEFAULT;
			//return PxFilterFlag::eSUPPRESS;
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		// #todo: enable continuous collision detection per entity
		const bool hasCCD0 = true;
		const bool hasCCD1 = true;
		if (hasCCD0 || hasCCD1)
			pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;

		// #todo: enable contact modifying per entity
		const bool hasContactModify0 = false;
		const bool hasContactModify1 = false;
		if (hasContactModify0 || hasContactModify1)
			pairFlags |= PxPairFlag::eMODIFY_CONTACTS;

		// #todo: enable contact notify per entity
		const bool hasContactNotify0 = true;
		const bool hasContactNotify1 = true;
		if (hasContactNotify0 || hasContactNotify1)
		{
			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		}

		return PxFilterFlag::eDEFAULT;
	}
}

physics::PhysicsManager::PhysicsManager()
{
}

physics::PhysicsManager::~PhysicsManager()
{
}

void physics::PhysicsManager::Initialise()
{
	physx::gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, physx::gAllocator, physx::gError);

	m_Debugger = physx::PxCreatePvd(*physx::gFoundation);
	m_Transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Debugger->connect(*m_Transport, physx::PxPvdInstrumentationFlag::eDEBUG);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *physx::gFoundation, physx::PxTolerancesScale(), false, m_Debugger);

	physx::gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);

	physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.cpuDispatcher = physx::gDispatcher;
	sceneDesc.filterShader = physx::contactReportFilterShader;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
	sceneDesc.gravity = physx::PxVec3(0.f, 0.f, 0.f);
	sceneDesc.simulationEventCallback = this;
	m_Scene = m_Physics->createScene(sceneDesc);
}

void physics::PhysicsManager::Destroy()
{
	m_Debugger->disconnect();

	m_Scene->release();
	m_Physics->release();

	m_Transport->release();
	m_Debugger->release();
	physx::gFoundation->release();
}

void physics::PhysicsManager::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
}

void physics::PhysicsManager::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	entt::entity entity0 = entt::entity(size_t(pairHeader.actors[0]->userData));
	entt::entity entity1 = entt::entity(size_t(pairHeader.actors[1]->userData));

	m_OnContactSignal.publish(entity0, entity1);
}

void physics::PhysicsManager::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
}

void physics::PhysicsManager::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
}

void physics::PhysicsManager::onWake(physx::PxActor** actors, physx::PxU32 count)
{
}