#pragma once

#include "Engine/AttributeTypes.h"

template<class Type, size_t N>
constexpr const char* reflect::GetFieldName(refl::field_descriptor<Type, N> descriptor)
{
	if constexpr (refl::descriptor::has_attribute<attr::Name>(descriptor))
	{
		attr::Name prettyName = refl::descriptor::get_attribute<attr::Name>(descriptor);
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