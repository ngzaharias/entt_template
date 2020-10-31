#include "PhysicsSystem.hpp"

#include "Math.hpp"
#include "Types.hpp"
#include "VectorHelpers.hpp"

#include "Components/Name.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"

#include <iostream>
#include <entt/entt.hpp>
#include <PhysX/PxPhysicsAPI.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

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

	static physx::UserAllocatorCallback gAllocator;
	static physx::UserErrorCallback gError;
	static physx::PxFoundation* gFoundation = nullptr;
	static physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
}

physics::PhysicsSystem::PhysicsSystem()
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
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = physx::gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);

	m_Material = m_Physics->createMaterial(0.f, 0.f, 1.f);

	registry.on_destroy<physics::RigidBody>().connect<&physics::PhysicsSystem::OnDestroy_RigidBody>(this);

	// ball
	{
		const physx::PxSphereGeometry geometry = physx::PxSphereGeometry(1.f);
		physx::PxShape* shape = m_Physics->createShape(geometry, *m_Material);

		const auto entity = registry.create();
		auto& name = registry.emplace<debug::Name>(entity);
		auto& rigidbody = registry.emplace<physics::RigidBody>(entity);
		auto& transform = registry.emplace<core::Transform>(entity);
		name.m_Name = "Ball";
		rigidbody.m_Actor = m_Physics->createRigidDynamic(physx::PxTransform({ 0.f, 10.f, 0.f }));
		rigidbody.m_Shapes.push_back(shape);

		for (auto* shape : rigidbody.m_Shapes)
			rigidbody.m_Actor->attachShape(*shape);
		m_Scene->addActor(*rigidbody.m_Actor);
	}

	// floor
	{
		const physx::PxBoxGeometry geometry = physx::PxBoxGeometry(10.f, 0.1f, 10.f);
		physx::PxShape* shape = m_Physics->createShape(geometry, *m_Material);

		const auto entity = registry.create();
		auto& name = registry.emplace<debug::Name>(entity);
		auto& rigidbody = registry.emplace<physics::RigidBody>(entity);
		auto& transform = registry.emplace<core::Transform>(entity);
		name.m_Name = "Floor";
		rigidbody.m_Actor = m_Physics->createRigidStatic(physx::PxTransform({ 0.f, 0.f, 0.f }));
		rigidbody.m_Shapes.push_back(shape);

		for (auto* shape : rigidbody.m_Shapes)
			rigidbody.m_Actor->attachShape(*shape);
		m_Scene->addActor(*rigidbody.m_Actor);
	}
}

void physics::PhysicsSystem::Destroy(entt::registry& registry)
{
	for (const entt::entity& entity : registry.view<physics::RigidBody>())
	{
		registry.destroy(entity);
	}

	registry.on_destroy<physics::RigidBody>().disconnect<&physics::PhysicsSystem::OnDestroy_RigidBody>(this);

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
	const float deltaTime = time.asSeconds();

	m_Scene->simulate(deltaTime);
	m_Scene->fetchResults(true);

	for (const entt::entity& entity : registry.view<physics::RigidBody, core::Transform>())
	{
		auto& rigidbody = registry.get<physics::RigidBody>(entity);
		auto& transform = registry.get<core::Transform>(entity);

		const physx::PxVec3 translate = rigidbody.m_Actor->getGlobalPose().p;
		transform.m_Translate = { translate.x, translate.y, translate.z };
	}
}

void physics::PhysicsSystem::OnDestroy_RigidBody(entt::registry& registry, entt::entity entity)
{
	auto& rigidbody = registry.get<physics::RigidBody>(entity);

	{
		const uint32 nbShapes = rigidbody.m_Actor->getNbShapes();
		physx::PxShape** shapes = new physx::PxShape * [nbShapes];
		rigidbody.m_Actor->getShapes(shapes, nbShapes);
		for (uint32 i = 0; i < nbShapes; ++i)
		{
			physx::PxShape* shape = shapes[i];
			rigidbody.m_Actor->detachShape(*shape);
			shape->release();
		}
		delete[] shapes;
	}

	rigidbody.m_Actor->release();
}
