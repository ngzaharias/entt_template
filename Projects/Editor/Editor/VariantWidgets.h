#pragma once

#include <variant>

namespace editor
{
	template<typename Descriptor, typename Type, typename Variant>
	void InspectVariant(const char* text, Descriptor descriptor, Variant& variant, Type& value);

	template<typename Descriptor, typename Type, typename ...Types>
	void InspectVariant(const char* text, Descriptor descriptor, std::variant<Types...>& variant, Type& value);
}

#include "VariantWidgets.inl"