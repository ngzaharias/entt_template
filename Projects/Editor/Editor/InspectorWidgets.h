#pragma once

namespace editor
{
	template<typename Type>
	void InspectType(Type& value);

	template<typename Type, typename Descriptor>
	void InspectProperty(const char* text, Type& value, Descriptor descriptor);

	template<typename Type, typename Descriptor>
	void InspectClass(const char* text, Type& value, Descriptor descriptor);
}

#include "InspectorWidgets.inl"