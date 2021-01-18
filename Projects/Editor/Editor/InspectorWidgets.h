#pragma once

namespace editor
{
	template<typename Type>
	void InspectType(Type& value);

	template<typename Type>
	void InspectField(const char* text, Type& value);

	template<typename Type>
	void FieldAsClass(const char* text, Type& value);
}

#include "InspectorWidgets.inl"