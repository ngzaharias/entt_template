#pragma once

namespace reflect
{
	template<class Descriptor>
	constexpr const char* GetName(Descriptor descriptor);
}

#include "AttributeHelpers.inl"