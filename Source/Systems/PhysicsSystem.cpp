#include "PhysicsSystem.hpp"

#include "Math.hpp"
#include "Screen.hpp"
#include "Types.hpp"
#include "VectorHelpers.hpp"

#include "Components/Camera.hpp"
#include "Components/Name.hpp"
#include "Components/RigidDynamic.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"

#include <iostream>
#include <entt/entt.hpp>
#include <PhysX/PxPhysicsAPI.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

namespace
{
	constexpr float s_Gravity = -9.81f * 100.f;
	constexpr int32 s_SimulationSubsteps = 1;
	constexpr float s_SimulationTimestep = 1.f / 60.f;
}

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

physics::PhysicsSystem::PhysicsSystem()
	: m_DeltaTimeAccumulated(0.f)
{
}

physics::PhysicsSystem::~PhysicsSystem()
{
}

void physics::PhysicsSystem::Initialize(entt::registry& registry)
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
	sceneDesc.gravity = physx::PxVec3(0.0f, s_Gravity, 0.0f);
	sceneDesc.simulationEventCallback = this;
	m_Scene = m_Physics->createScene(sceneDesc);

	m_Material = m_Physics->createMaterial(0.f, 0.f, 0.5f);

	registry.on_destroy<physics::RigidDynamic>().connect<&physics::PhysicsSystem::OnDestroy_RigidBody>(this);
}

void physics::PhysicsSystem::Destroy(entt::registry& registry)
{
	for (const entt::entity& entity : registry.view<physics::RigidDynamic>())
	{
		registry.destroy(entity);
	}

	registry.on_destroy<physics::RigidDynamic>().disconnect<&physics::PhysicsSystem::OnDestroy_RigidBody>(this);

	m_Debugger->disconnect();

	m_Material->release();
	m_Scene->release();
	m_Physics->release();

	m_Transport->release();
	m_Debugger->release();
	physx::gFoundation->release();
}

void physics::PhysicsSystem::Update(entt::registry& registry, const sf::Time& time)
{
	m_DeltaTimeAccumulated += time.asSeconds();

	for (; m_DeltaTimeAccumulated >= s_SimulationTimestep; m_DeltaTimeAccumulated -= s_SimulationTimestep)
	{
		constexpr float dt = s_SimulationTimestep / s_SimulationSubsteps;
		for (int32 i = 0; i < s_SimulationSubsteps; ++i)
		{
			m_Scene->simulate(dt);
			m_Scene->fetchResults(true);
		}
	}

	for (const entt::entity& entity : registry.view<physics::RigidDynamic, core::Transform>())
	{
		auto& rigidbody = registry.get<physics::RigidDynamic>(entity);
		auto& transform = registry.get<core::Transform>(entity);

		const physx::PxVec3 translate = rigidbody.m_Actor->getGlobalPose().p;
		transform.m_Translate = { translate.x, translate.y, translate.z };
	}
}

void physics::PhysicsSystem::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
}

void physics::PhysicsSystem::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	entt::entity entity0 = entt::entity(size_t(pairHeader.actors[0]->userData));
	entt::entity entity1 = entt::entity(size_t(pairHeader.actors[1]->userData));
	m_OnContact.emplace_back(entity0);
	m_OnContact.emplace_back(entity1);
}

void physics::PhysicsSystem::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
}

void physics::PhysicsSystem::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
}

void physics::PhysicsSystem::onWake(physx::PxActor** actors, physx::PxU32 count)
{
}

void physics::PhysicsSystem::OnDestroy_RigidBody(entt::registry& registry, entt::entity entity)
{
	auto& rigidbody = registry.get<physics::RigidDynamic>(entity);

	for (auto* shape : rigidbody.m_Shapes)
	{
		rigidbody.m_Actor->detachShape(*shape);
		shape->release();
	}

	rigidbody.m_Actor->release();
}
