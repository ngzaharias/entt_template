#pragma once

namespace widget
{
	template<typename Type>
	void TypeAsIs(Type& value);

	template<>
	void TypeAsIs(bool& value);

	template<>
	void TypeAsIs(int& value);

	template<>
	void TypeAsIs(float& value);

	template<>
	void TypeAsIs(Vector3f& value);
}

#include "TrivialWidgets.inl"