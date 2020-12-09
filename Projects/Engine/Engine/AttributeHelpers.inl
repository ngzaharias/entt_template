#pragma once

#include "Engine/FieldAttributes.h"

template<class Descriptor>
constexpr const char* reflect::GetName(Descriptor descriptor)
{
	if constexpr (refl::descriptor::has_attribute<field::Name>(descriptor))
	{
		field::Name prettyName = refl::descriptor::get_attribute<field::Name>(descriptor);
		return prettyName.m_Value;
	}
	else
	{
		return get_display_name(descriptor);
	}
}