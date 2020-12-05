#pragma once

#include <Editor/PropertyDefines.h>

#include <entt/meta/meta.hpp>
#include <SFML/System/Vector3.hpp>

namespace editor
{
	template<class Type>
	void PropertyWidget(editor::Any propertyData, const entt::meta_data& metaData);

	void PropertyWidget_Child(entt::meta_any propertyData, const entt::meta_data& metaData = nullptr);

	void PropertyWidget_AssociativeContainer(entt::meta_any propertyData, const entt::meta_data& metaData = nullptr);
	void PropertyWidget_SequenceContainer(entt::meta_any propertyData, const entt::meta_data& metaData = nullptr);
}

#include "PropertyWidgets.inl"