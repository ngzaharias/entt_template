#pragma once

#include <refl/refl.hpp>

namespace
{
	template <typename Type, typename Variant>
	void VisitVariant(serialize::Reader& reader, Variant& value, uint32& i, const uint32 index)
	{
		if (i++ == index)
		{
			Type val;
			reader.Visit(val);
			value = val;
		}
	}
}

template<typename Type>
void serialize::Reader::Visit(Type& value)
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
		assert(false);
	}
}

template<typename Key, typename Val>
void serialize::Reader::Visit(Map<Key, Val>& value)
{
	uint32 size;
	Visit(size);

	Key key; Val val;
	for (uint32 i = 0; i < size; ++i)
	{
		Visit(key);
		Visit(val);
		value[key] = val;
	}
}

template<typename Type>
void serialize::Reader::Visit(Set<Type>& value)
{
	uint32 size;
	Visit(size);

	Type val;
	for (uint32 i = 0; i < size; ++i)
	{
		Visit(val);
		value.push_back(val);
	}
}

template<typename Type>
void serialize::Reader::Visit(Array<Type>& value)
{
	uint32 size;
	Visit(size);
	value.reserve(size);

	Type val;
	for (uint32 i = 0; i < size; ++i)
	{
		Visit(val);
		value.push_back(val);
	}
}

template<typename Type>
void serialize::Reader::Visit(Nullable<Type>& value)
{
	bool hasValue;
	Visit(hasValue);
	if (hasValue)
	{
		Type val;
		Visit(val);
		value = val;
	}
}

template<typename ...Types>
void serialize::Reader::Visit(Variant<Types...>& value)
{
	uint32 i = 0;
	uint32 index;
	Visit(index);
	(VisitVariant<Types>(*this, value, i, index), ...);
}
