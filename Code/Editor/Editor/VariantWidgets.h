#pragma once

#include <variant>

namespace editor
{
	template<typename Type, typename Variant>
	void InspectVariant(Variant& variant, Type& value, InspectorInfo& info);

	template<typename Type, typename ...Types>
	void InspectVariant(std::variant<Types...>& variant, Type& value, InspectorInfo& info);
}

#include "VariantWidgets.inl"