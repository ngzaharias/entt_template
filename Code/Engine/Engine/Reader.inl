#pragma once

#include <refl/refl.hpp>

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

template<typename Type>
void serialize::Reader::Visit(std::vector<Type>& value)
{
	uint32 size;
	Visit(size);
	value.reserve(size);

	Type child;
	for (uint32 i = 0; i < size; ++i)
	{
		Visit(child);
		value.push_back(child);
	}
}