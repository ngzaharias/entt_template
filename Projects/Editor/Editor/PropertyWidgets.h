#pragma once

#include <Editor/PropertyDefines.h>

#include <entt/meta/meta.hpp>
#include <SFML/System/Vector3.hpp>

namespace editor
{
	template<class Type>
	void PropertyWidget(editor::Any propertyData, const entt::meta_data& metaData);
}

#include "PropertyWidgets.inl"