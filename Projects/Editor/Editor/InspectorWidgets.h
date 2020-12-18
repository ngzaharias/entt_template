#pragma once

namespace editor
{
	template<typename Type>
	void Field(const char* text, Type& value);

	template<typename Type>
	void FieldAsClass(const char* text, Type& value);
}

#include "InspectorWidgets.inl"