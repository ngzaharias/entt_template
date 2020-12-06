#pragma once

#include <SFML/System/Vector3.hpp>

namespace editor
{
	template<class Descriptor, class Type>
	void PropertyWidget(const Descriptor& descriptor, Type& value);

	template<class Descriptor>
	void PropertyWidget(const Descriptor& descriptor, bool& value);

	template<class Descriptor>
	void PropertyWidget(const Descriptor& descriptor, int& value);

	template<class Descriptor>
	void PropertyWidget(const Descriptor& descriptor, float& value);

	template<class Descriptor>
	void PropertyWidget(const Descriptor& descriptor, sf::Vector3f& value);
}

#include "PropertyWidgets.inl"