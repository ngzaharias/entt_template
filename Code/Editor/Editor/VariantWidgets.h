#pragma once

namespace editor
{
	template<typename Type, typename TVariant>
	void InspectVariant(TVariant& variant, Type& value, InspectorInfo& info);

	template<typename Type, typename ...Types>
	void InspectVariant(Variant<Types...>& variant, Type& value, InspectorInfo& info);
}

#include "VariantWidgets.inl"