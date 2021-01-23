#pragma once

namespace editor
{
	template<typename Type>
	void InspectType(Type& value);

	template<typename Descriptor, typename Type>
	void InspectMember(const char* text, Descriptor descriptor, Type& value);

	template<typename Type>
	void InspectClass(const char* text, Type& value);
}

#include "InspectorWidgets.inl"