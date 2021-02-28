#pragma once

#include <Editor/ExampleComponent.h>

#include <Engine/CameraComponent.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/InputComponent.h>
#include <Engine/LevelComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/RigidDynamicComponent.h>
#include <Engine/RigidStaticComponent.h>
#include <Engine/SoundComponent.h>
#include <Engine/SpriteComponent.h>
#include <Engine/TransformComponent.h>

namespace editor
{
	using ComponentsList = core::TypeList
		<
		core::NameComponent
		, core::TransformComponent

		, audio::SoundComponent
		, core::CameraComponent
		, core::InputComponent
		, core::LevelComponent
		, example::ExampleComponent
		, physics::RigidDynamicComponent
		, physics::RigidStaticComponent
		, render::FlipbookComponent
		, render::SpriteComponent
		>;

	constexpr ComponentsList s_ComponentList;
}