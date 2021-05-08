#include "EnginePCH.h"
#include "Engine/PhysicsSystem.h"

#include "Engine/CameraComponent.h"
#include "Engine/MathHelpers.h"
#include "Engine/NameComponent.h"
#include "Engine/PhysicsManager.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/Screen.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/VectorHelpers.h"

#include <entt/entt.hpp>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxScene.h>

namespace
{
	constexpr float s_Gravity = -9.81f * 100.f;
	constexpr int32 s_SimulationSubsteps = 1;
	constexpr float s_SimulationTimestep = 1.f / 60.f;
}

physics::PhysicsSystem::PhysicsSystem(physics::PhysicsManager& physicsManager)
	: m_PhysicsManager(physicsManager)
{
}

void physics::PhysicsSystem::Initialise()
{
	auto& registry = m_World->m_Registry;

	m_PhysicsManager.GetScene().setGravity(physx::PxVec3(0.f, s_Gravity, 0.f));

	entt::sink(m_PhysicsManager.m_OnContactSignal).connect<&physics::PhysicsSystem::OnContact>(this);

	registry.on_destroy<physics::RigidDynamicComponent>().connect<&physics::PhysicsSystem::OnDestroy_RigidBody>(this);
}

void physics::PhysicsSystem::Destroy()
{
	auto& registry = m_World->m_Registry;

	for (const entt::entity& entity : registry.view<physics::RigidDynamicComponent>())
	{
		registry.destroy(entity);
	}

	registry.on_destroy<physics::RigidDynamicComponent>().disconnect<&physics::PhysicsSystem::OnDestroy_RigidBody>(this);
}

void physics::PhysicsSystem::Update(const core::GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& registry = m_World->m_Registry;

	m_DeltaTimeAccumulated += gameTime.asSeconds();

	for (; m_DeltaTimeAccumulated >= s_SimulationTimestep; m_DeltaTimeAccumulated -= s_SimulationTimestep)
	{
		constexpr float dt = s_SimulationTimestep / s_SimulationSubsteps;
		for (int32 i = 0; i < s_SimulationSubsteps; ++i)
		{
			m_PhysicsManager.GetScene().simulate(dt);
			m_PhysicsManager.GetScene().fetchResults(true);
		}
	}

	for (const entt::entity& entity : registry.view<physics::RigidDynamicComponent, core::TransformComponent>())
	{
		auto& rigidDynamic = registry.get<physics::RigidDynamicComponent>(entity);
		auto& transform = registry.get<core::TransformComponent>(entity);

		const physx::PxVec3 translate = rigidDynamic.m_Actor->getGlobalPose().p;
		transform.m_Translate = { translate.x, translate.y, translate.z };
	}
}

void physics::PhysicsSystem::OnContact(const entt::entity& entityA, const entt::entity& entityB)
{
	m_OnCollideSignal.publish(entityA, entityB);
}

void physics::PhysicsSystem::OnDestroy_RigidBody(entt::registry& registry, entt::entity entity)
{
	auto& rigidDynamic = registry.get<physics::RigidDynamicComponent>(entity);

	for (auto* shape : rigidDynamic.m_Shapes)
	{
		rigidDynamic.m_Actor->detachShape(*shape);
		shape->release();
	}

	rigidDynamic.m_Actor->release();
}
