#pragma once

namespace editor
{
	template<typename FieldDescriptor, typename Type>
	void InspectorField(const FieldDescriptor fieldDescriptor, Type& value);

	//////////////////////////////////////////////////////////////////////////

	template<typename Type>
	void InspectorType(Type& value);

	template<>
	void InspectorType(bool& value);

	template<>
	void InspectorType(int& value);

	template<>
	void InspectorType(float& value);

	template<>
	void InspectorType(sf::Vector3f& value);
}

#include "InspectorWidgets.inl"