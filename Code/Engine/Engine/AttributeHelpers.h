#pragma once

#include <refl/refl.hpp>

namespace reflect
{
	template<class Type, size_t N>
	constexpr const char* GetFieldName(refl::field_descriptor<Type, N> descriptor);

	template<class Type>
	constexpr const char* GetTypeName();

	template<class Type>
	constexpr const char* GetTypeName(refl::type_descriptor<Type> descriptor);
}

#include "AttributeHelpers.inl"