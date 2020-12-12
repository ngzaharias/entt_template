#pragma once

#include "Engine/FieldAttributes.h"

template<class Type, size_t N>
constexpr const char* reflect::GetFieldName(refl::field_descriptor<Type, N> descriptor)
{
	if constexpr (refl::descriptor::has_attribute<field::Name>(descriptor))
	{
		field::Name prettyName = refl::descriptor::get_attribute<field::Name>(descriptor);
		return prettyName.m_Value;
	}
	return descriptor.name.c_str();
}

template<class Type>
constexpr const char* reflect::GetTypeName()
{
	return refl::reflect<Type>().name.c_str();
}

template<class Type>
constexpr const char* reflect::GetTypeName(refl::type_descriptor<Type> descriptor)
{
	return descriptor.name.c_str();
}