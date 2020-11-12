#pragma once

namespace math
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

	inline float Sign(float value)
	{
		return (value < 0.0f) ? -1.0f : 1.0f;
	}

	inline float ToDegrees(float radians)
	{
		return radians * 57.2958f;
	}

	inline float ToRadians(float degrees)
	{
		return degrees * 0.0174533f;
	}
}
