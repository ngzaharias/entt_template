#pragma once

namespace Math
{
	template<typename T> T Clamp(const T& value, const T& min, const T& max)
	{
		return value < min ? min : value > max ? max : value;
	}

	template<typename T> T Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	template<typename T> T Max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	float Sign(float value);
}
