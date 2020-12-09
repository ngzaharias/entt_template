#pragma once

#include <map>
#include <vector>
#include <SFML/System/Vector3.hpp>

namespace editor
{
	template<typename Descriptor, typename Type>
	void Field(const Descriptor descriptor, Type& value);

	//////////////////////////////////////////////////////////////////////////

	template<class Descriptor, class Type>
	void FieldAsClass(const Descriptor descriptor, Type& value);

	//////////////////////////////////////////////////////////////////////////

	template<class Descriptor, class Container>
	void FieldAsContainer(const Descriptor descriptor, Container& container);

	template<class Descriptor, class Key, class Value>
	void FieldAsContainer(const Descriptor descriptor, std::map<Key, Value>& container);

	template<class Descriptor, class Type>
	void FieldAsContainer(const Descriptor descriptor, std::vector<Type>& container);

	//////////////////////////////////////////////////////////////////////////

	template<class Type>
	void PropertyWidget(Type& value);

	template<>
	void PropertyWidget(bool& value);

	template<>
	void PropertyWidget(int& value);

	template<>
	void PropertyWidget(float& value);

	template<>
	void PropertyWidget(sf::Vector3f& value);
}

#include "PropertyWidgets.inl"