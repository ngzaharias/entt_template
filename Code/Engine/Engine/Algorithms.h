#pragma once

#include <Engine/Set.h>

namespace core
{
	template<typename Type>
	void Difference(const Set<Type>& inputA, const Set<Type>& inputB, Set<Type>& output);

	template<typename Type>
	void Intersection(const Set<Type>& inputA, const Set<Type>& inputB, Set<Type>& output);
}

#include "Algorithms.inl"