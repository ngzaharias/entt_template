#include "example_attributes.h"

#include <iostream>
#include <refl/refl.hpp>

struct Attr : refl::attr::usage::any
{
	constexpr Attr(const int value) : m_Int(value) { }
	const int m_Int = 0;
};

struct MyStruct { float m_Value = true; };

REFL_AUTO(type(MyStruct), field(m_Value, Attr(1)))

template<class Descriptor, class Value>
constexpr void DoFunction(Descriptor desc, Value value)
{
	// doesn't compile
	if constexpr (refl::descriptor::has_attribute<Attr>(desc))
		std::cout << "has_attribute\n";
}

void example_attributes()
{
	MyStruct value;
	for_each(refl::reflect<MyStruct>().members, [&](auto field)
	{
		// compiles
		if constexpr (refl::descriptor::has_attribute<Attr>(field))
			std::cout << "has_attribute\n";

		DoFunction(field, field(value));
	});
}