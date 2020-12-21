#pragma once

#include <Editor/TrivialWidgets.h>

#include <Engine/PhysicsMaterialResource.h>
#include <Engine/SoundResource.h>
#include <Engine/TextureResource.h>

namespace widget
{
	template<>
	void TypeAsIs(physics::MaterialHandle& value);

	template<>
	void TypeAsIs(audio::SoundHandle& value);

	template<>
	void TypeAsIs(render::TextureHandle& value);
}

#include "ResourceWidgets.inl"