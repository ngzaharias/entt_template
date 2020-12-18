#pragma once

#include <variant>

namespace widget
{
	template<typename Type, typename Variant>
	void FieldAsVariant(const char* text, Variant& variant, Type& value);

	template<typename Type, typename ...Types>
	void FieldAsVariant(const char* text, std::variant<Types...>& variant, Type& value);
}

#include "VariantWidgets.inl"