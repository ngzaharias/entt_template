#pragma once

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

	template<class Descriptor, class Variant>
	void FieldAsVariant(const Descriptor descriptor, Variant& variant);

	template<class Descriptor, typename ...Types>
	void FieldAsVariant(const Descriptor descriptor, std::variant<Types...>& variant);

	//////////////////////////////////////////////////////////////////////////

	template<class Type>
	void PropertyWidget(const char* name, Type& value);

	template<>
	void PropertyWidget(const char* name, bool& value);

	template<>
	void PropertyWidget(const char* name, int& value);

	template<>
	void PropertyWidget(const char* name, float& value);

	template<>
	void PropertyWidget(const char* name, sf::Vector3f& value);
}

#include "PropertyWidgets.inl"