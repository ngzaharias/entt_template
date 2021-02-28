#pragma once

#include <Editor/InspectorTypes.h>

namespace editor
{
	template<typename Type>
	void InspectType(Type& value, InspectorInfo& info);

	template<typename Type>
	void InspectProperty(Type& value, InspectorInfo& info);

	template<typename Type>
	void InspectClass(Type& value, InspectorInfo& info);
}

#include "InspectorWidgets.inl"