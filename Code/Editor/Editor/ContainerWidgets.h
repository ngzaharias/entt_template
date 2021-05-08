#pragma once

#include <Editor/InspectorTypes.h>

namespace editor
{
	template<class Container>
	void InspectContainer(Container& container, InspectorInfo& info);

	template<class Key, class Value>
	void InspectContainer(Map<Key, Value>& container, InspectorInfo& info);

	template<typename Type>
	void InspectContainer(Array<Type>& container, InspectorInfo& info);
}

#include "ContainerWidgets.inl"