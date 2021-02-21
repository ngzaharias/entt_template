#pragma once

#include <map>
#include <vector>

namespace editor
{
	template<typename Descriptor, class Container>
	void InspectContainer(const char* text, Descriptor descriptor, Container& container);

	template<typename Descriptor, class Key, class Value>
	void InspectContainer(const char* text, Descriptor descriptor, std::map<Key, Value>& container);

	template<typename Descriptor, typename Type>
	void InspectContainer(const char* text, Descriptor descriptor, std::vector<Type>& container);
}

#include "ContainerWidgets.inl"