#pragma once

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
void serialize::Writer::Visit(const std::vector<Type>& value)
{
	const uint32 size = static_cast<uint32>(value.size());
	Visit(size);

	for (uint32 i = 0; i < size; ++i)
		Visit(value[i]);
}