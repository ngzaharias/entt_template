#pragma once

#include <Editor/InspectorTypes.h>

#include <map>
#include <vector>

namespace editor
{
	template<class Container>
	void InspectContainer(Container& container, InspectorInfo& info);

	template<class Key, class Value>
	void InspectContainer(std::map<Key, Value>& container, InspectorInfo& info);

	template<typename Type>
	void InspectContainer(std::vector<Type>& container, InspectorInfo& info);
}

#include "ContainerWidgets.inl"