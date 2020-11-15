#include "Engine/LevelSystem.h"

#include "Engine/CameraComponent.h"
#include "Engine/JsonHelpers.h"
#include "Engine/LevelComponent.h"
#include "Engine/MathHelpers.h"
#include "Engine/Name.h"
#include "Engine/NameComponent.h"
#include "Engine/Path.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PhysicsMaterialResource.h"
#include "Engine/ResourceManager.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Types.h"

#include <filesystem>
#include <entt/entt.hpp>
#include <rapidjson/document.h>
#include <PhysX/PxMaterial.h>
#include <PhysX/PxPhysics.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>
#include <PhysX/PxScene.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace
{
	str::Guid strDefaultMaterial = "a4835493-ae5a-40ba-8083-06deb381c801";
}

core::LevelSystem::LevelSystem
(
	physics::PhysicsManager& physicsManager
	, core::ResourceManager& resourceManager
)
	: m_PhysicsManager(physicsManager)
	, m_ResourceManager(resourceManager)
{
}

core::LevelSystem::~LevelSystem()
{
}

void core::LevelSystem::Initialize(entt::registry& registry)
{
}

void core::LevelSystem::Destroy(entt::registry& registry)
{
}

void core::LevelSystem::Update(entt::registry& registry, const sf::Time& time)
{
}

bool core::LevelSystem::Load(entt::registry& registry, const str::Path& directory)
{
	for (const auto& entry : std::filesystem::directory_iterator(directory.ToChar()))
	{
		const str::Path filepath = entry.path().string();
		const entt::entity entity = CreateEntity(registry, filepath.ToChar());

		core::LevelComponent& levelComponent = registry.emplace<core::LevelComponent>(entity);
		levelComponent.m_Name = filepath.ToChar();
		levelComponent.m_Path = filepath.ToChar();
	}

	return true;
}

void core::LevelSystem::Unload(entt::registry& registry)
{
}

entt::entity core::LevelSystem::CreateEntity(entt::registry& registry, const char* filepath)
{
	rapidjson::Document document;
	if (!json::LoadDocument(filepath, document))
		return entt::null;

	const entt::entity entity = registry.create();

	// name
	{
		debug::NameComponent& name = registry.emplace<debug::NameComponent>(entity);
		name.m_Name = json::ParseString(document, "name", "<unknown>");
	}

	// transform
	if (auto itr_transform = document.FindMember("transform"); itr_transform != document.MemberEnd())
	{
		core::TransformComponent& transform = registry.emplace<core::TransformComponent>(entity);

		if (auto itr_translate = itr_transform->value.FindMember("translate");  itr_translate != itr_transform->value.MemberEnd())
		{
			const float32 x = itr_translate->value["x"].GetFloat();
			const float32 y = itr_translate->value["y"].GetFloat();
			const float32 z = itr_translate->value["z"].GetFloat();
			transform.m_Translate = { x, y, z };
		}

		if (auto itr_rotate = itr_transform->value.FindMember("rotate"); itr_rotate != itr_transform->value.MemberEnd())
		{
			const float32 x = itr_rotate->value["x"].GetFloat();
			const float32 y = itr_rotate->value["y"].GetFloat();
			const float32 z = itr_rotate->value["z"].GetFloat();
			transform.m_Rotate = { x, y, z };
		}

		if (auto itr_scale = itr_transform->value.FindMember("scale"); itr_scale != itr_transform->value.MemberEnd())
		{
			const float32 x = itr_scale->value["x"].GetFloat();
			const float32 y = itr_scale->value["y"].GetFloat();
			const float32 z = itr_scale->value["z"].GetFloat();
			transform.m_Scale = { x, y, z };
		}
	}

	// camera
	if (auto itr_camera = document.FindMember("camera"); itr_camera != document.MemberEnd())
	{
		core::CameraComponent& camera = registry.emplace<core::CameraComponent>(entity);
		camera.m_Size.x = static_cast<float32>(json::ParseInt(itr_camera->value, "width", 800));
		camera.m_Size.y = static_cast<float32>(json::ParseInt(itr_camera->value, "height", 600));
	}

	// physics
	if (auto itr_physics = document.FindMember("physics"); itr_physics != document.MemberEnd())
	{
		physx::PxPhysics& physics = m_PhysicsManager.GetPhysics();
		physx::PxScene& scene = m_PhysicsManager.GetScene();
		physx::PxShape* shape = nullptr;

		physx::PxVec3 translate = physx::PxVec3(0.f);
		physx::PxQuat rotate = physx::PxQuat(physx::PxIdentity);
		physx::PxVec3 scale = physx::PxVec3(1.f);
		if (core::TransformComponent* component = registry.try_get<core::TransformComponent>(entity))
		{
			const float32 radianX = math::ToRadians(component->m_Rotate.x);
			const float32 radianY = math::ToRadians(component->m_Rotate.y);
			const float32 radianZ = math::ToRadians(component->m_Rotate.z);
			const physx::PxQuat quatX = physx::PxQuat(radianX, { 1.f, 0.f, 0.f });
			const physx::PxQuat quatY = physx::PxQuat(radianY, { 0.f, 1.f, 0.f });
			const physx::PxQuat quatZ = physx::PxQuat(radianZ, { 0.f, 0.f, 1.f });

			translate = { component->m_Translate.x, component->m_Translate.y, component->m_Translate.z };
			rotate = quatX * quatY * quatZ;
			scale = { component->m_Scale.x, component->m_Scale.y, component->m_Scale.z };
		}

		if (auto itr_shape = itr_physics->value.FindMember("shape"); itr_shape != itr_physics->value.MemberEnd())
		{
			const physics::MaterialHandle handle = m_ResourceManager.GetResource<physics::MaterialResource>(strDefaultMaterial);

			const int32 world0 = json::ParseInt(itr_shape->value, "channel", 0);
			const physx::PxFilterData filterData = physx::PxFilterData(world0, 0, 0, 0);

			if (auto itr_box = itr_shape->value.FindMember("box"); itr_box != itr_shape->value.MemberEnd())
			{
				const float32 extents_x = itr_box->value["extents_x"].GetFloat() * scale.x;
				const float32 extents_y = itr_box->value["extents_y"].GetFloat() * scale.y;
				const float32 extents_z = itr_box->value["extents_z"].GetFloat() * scale.z;

				shape = physics.createShape(physx::PxBoxGeometry(extents_x, extents_y, extents_z), *handle->m_Material);
				shape->setSimulationFilterData(filterData);
			}
			else if (auto itr_sphere = itr_shape->value.FindMember("sphere"); itr_sphere != itr_shape->value.MemberEnd())
			{
				const float32 radius = json::ParseFloat(itr_sphere->value, "radius", 10.f);

				// #todo: move creation to PhysicsManager
				shape = physics.createShape(physx::PxSphereGeometry(radius * scale.x), *handle->m_Material);
				shape->setSimulationFilterData(filterData);
			}
		}

		if (auto itr_dynamic = itr_physics->value.FindMember("dynamic"); itr_dynamic != itr_physics->value.MemberEnd())
		{
			physics::RigidDynamicComponent& rigidDynamic = registry.emplace<physics::RigidDynamicComponent>(entity);
			rigidDynamic.m_Actor = physics.createRigidDynamic({ translate, rotate });
			rigidDynamic.m_Actor->userData = reinterpret_cast<void*>(entity);
			rigidDynamic.m_Shapes.push_back(shape);

			const bool eENABLE_CCD = json::ParseBool(itr_dynamic->value, "eENABLE_CCD", false);
			const bool eLOCK_ANGULAR_X = json::ParseBool(itr_dynamic->value, "eLOCK_ANGULAR_X", false);
			const bool eLOCK_ANGULAR_Y = json::ParseBool(itr_dynamic->value, "eLOCK_ANGULAR_Y", false);
			const bool eLOCK_ANGULAR_Z = json::ParseBool(itr_dynamic->value, "eLOCK_ANGULAR_Z", false);
			const bool eLOCK_LINEAR_X = json::ParseBool(itr_dynamic->value, "eLOCK_LINEAR_X", false);
			const bool eLOCK_LINEAR_Y = json::ParseBool(itr_dynamic->value, "eLOCK_LINEAR_Y", false);
			const bool eLOCK_LINEAR_Z = json::ParseBool(itr_dynamic->value, "eLOCK_LINEAR_Z", false);

			rigidDynamic.m_Actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, eENABLE_CCD);
			rigidDynamic.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, eLOCK_ANGULAR_X);
			rigidDynamic.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, eLOCK_ANGULAR_Y);
			rigidDynamic.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, eLOCK_ANGULAR_Z);
			rigidDynamic.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, eLOCK_LINEAR_X);
			rigidDynamic.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, eLOCK_LINEAR_Y);
			rigidDynamic.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, eLOCK_LINEAR_Z);

			for (auto* shape : rigidDynamic.m_Shapes)
				rigidDynamic.m_Actor->attachShape(*shape);
			scene.addActor(*rigidDynamic.m_Actor);
		}
		else if (auto itr_static = itr_physics->value.FindMember("static"); itr_static != itr_physics->value.MemberEnd())
		{
			physics::RigidStaticComponent& rigidStatic = registry.emplace<physics::RigidStaticComponent>(entity);
			rigidStatic.m_Actor = physics.createRigidStatic({ translate, rotate });
			rigidStatic.m_Actor->userData = reinterpret_cast<void*>(entity);
			rigidStatic.m_Shapes.push_back(shape);

			for (auto* shape : rigidStatic.m_Shapes)
				rigidStatic.m_Actor->attachShape(*shape);
			scene.addActor(*rigidStatic.m_Actor);
		}
	}

	// sprite
	if (auto itr_sprite = document.FindMember("sprite"); itr_sprite != document.MemberEnd())
	{
		render::SpriteComponent& sprite = registry.emplace<render::SpriteComponent>(entity);

		if (auto itr_size = itr_sprite->value.FindMember("size"); itr_size != itr_sprite->value.MemberEnd())
		{
			const float32 width = json::ParseFloat(itr_size->value, "width", 32.f);
			const float32 height = json::ParseFloat(itr_size->value, "height", 32.f);
			sprite.m_Size = { width, height };
		}

		if (auto itr_texture = itr_sprite->value.FindMember("texture"); itr_texture != itr_sprite->value.MemberEnd())
		{
			const str::Guid guid = itr_texture->value.GetString();
			const render::TextureHandle handle = m_ResourceManager.GetResource<render::TextureResource>(guid);

			sprite.m_Handle = handle;
			sprite.m_Sprite.setTexture(handle->m_Texture);
			sprite.m_Sprite.setOrigin(sf::Vector2f(handle->m_Texture.getSize()) * 0.5f);
		}
	}

	return entity;
}
