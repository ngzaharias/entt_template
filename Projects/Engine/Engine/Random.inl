#pragma once

#include <random>

template<typename Type>
Type random::Range<Type>(const Type& min, const Type& max)
{
	if (min == max)
		return min;

	const Type r = std::rand() % (max - min + 1);
	return r + min;
}

template<>
float random::Range<float>(const float& min, const float& max)
{
	if (min == max)
		return min;

	const float r = std::rand() / (float)RAND_MAX;
	return (r * (max - min)) + min;
}