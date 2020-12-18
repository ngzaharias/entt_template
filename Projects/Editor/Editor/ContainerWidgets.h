#pragma once

#include <map>
#include <vector>

namespace widget
{
	template<class Container>
	void FieldAsContainer(const char* text, Container& container);

	template<class Key, class Value>
	void FieldAsContainer(const char* text, std::map<Key, Value>& container);

	template<typename Type>
	void FieldAsContainer(const char* text, std::vector<Type>& container);
}

#include "ContainerWidgets.inl"