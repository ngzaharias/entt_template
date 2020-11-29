#pragma once

#include <Editor/PropertyDefines.h>

#include <entt/meta/meta.hpp>
#include <SFML/System/Vector3.hpp>

namespace editor
{
	template<class Type>
	void PropertyWidget(Void propertyData, const entt::meta_data& metaData);

	template<>
	void PropertyWidget<bool>(Void propertyData, const entt::meta_data& metaData);

	template<>
	void PropertyWidget<int>(Void propertyData, const entt::meta_data& metaData);

	template<>
	void PropertyWidget<sf::Vector3f>(Void propertyData, const entt::meta_data& metaData);
}

#include "PropertyWidgets.inl"