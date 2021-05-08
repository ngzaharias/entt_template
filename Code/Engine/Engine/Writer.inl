#pragma once

#include <Engine/AttributeTypes.h>

#include <refl/refl.hpp>

template<typename Type>
void serialize::Writer::Visit(const Type& value)
{
	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	if constexpr (isClass && isReflectable)
	{
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			Visit(field(value));
		});
	}
	else
	{
		static_assert(false, "Type isn't supported!");
	}
}

template<typename Type>
void serialize::Writer::Visit(const Array<Type>& value)
{
	Visit(static_cast<uint32>(value.size()));
	for (auto&& val : value)
		Visit(val);
}

template<typename Key, typename Val>
void serialize::Writer::Visit(const Map<Key, Val>& value)
{
	Visit(static_cast<uint32>(value.size()));
	for (auto&& [key, val] : value)
	{
		Visit(key);
		Visit(val);
	}
}

template<typename Type>
void serialize::Writer::Visit(const Set<Type>& value)
{
	Visit(static_cast<uint32>(value.size()));
	for (const Type& val : value)
		Visit(val);
}

template<typename Type>
void serialize::Writer::Visit(const Nullable<Type>& value)
{
	Visit(value.has_value());
	if (value.has_value())
	{
		Visit(value.value());
	}
}

template<typename ...Types>
void serialize::Writer::Visit(const Variant<Types...>& value)
{
	Visit(static_cast<uint32>(value.index()));
	std::visit([&](auto&& val)
	{
		Visit(val);
	}, value);
}