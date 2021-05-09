#pragma once

#include <algorithm>

template<typename Type>
void core::Difference(const Set<Type>& inputA, const Set<Type>& inputB, Set<Type>& output)
{
	std::set_difference
	(
		inputA.cbegin(),
		inputA.cend(),
		inputB.cbegin(),
		inputB.cend(),
		std::inserter(output, output.end())
	);
}

template<typename Type>
void core::Intersection(const Set<Type>& inputA, const Set<Type>& inputB, Set<Type>& output)
{
	std::set_intersection
	(
		inputA.cbegin(),
		inputA.cend(),
		inputB.cbegin(),
		inputB.cend(),
		std::inserter(output, output.end())
	);
}