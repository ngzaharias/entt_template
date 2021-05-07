#pragma once

#define KINDA_LARGE 9999999.f
#define KINDA_SMALL .0000001f

#define TWO_PI (6.2831853071795864769252867665590f)
#define ONE_PI (3.1415926535897932384626433832795f)
#define HALF_PI (1.5707963267948966192313216916398f)

#define SQUARE_ROOT_TWO (1.4142135623730950488016887242097f)

namespace math
{
	template<typename Type>
	constexpr Type Clamp(const Type& value, const Type& min, const Type& max)
	{
		return value < min ? min : value > max ? max : value;
	}

	template<typename Type>
	constexpr Type Lerp(const Type& a, const Type& b, const float t)
	{
		return a + (b - a) * t;
	}

	template<typename Type>
	constexpr Type Min(const Type& a, const Type& b)
	{
		return (a < b) ? a : b;
	}

	template<typename Type>
	constexpr Type Max(const Type& a, const Type& b)
	{
		return (a > b) ? a : b;
	}

	template<typename Type>
	constexpr Type Remap(Type value, const Type& fromA, const Type& fromB, const Type& toA, const Type& toB)
	{
		value -= fromA;
		value /= (fromB - fromA);
		return value * (toB - toA) + toA;
	}

	constexpr float Sign(float value)
	{
		return (value < 0.0f) ? -1.0f : 1.0f;
	}

	template<typename Type>
	constexpr float Sqr(const Type& value)
	{
		return value * value;
	}

	constexpr float ToDegrees(float radians)
	{
		return radians * 57.2958f;
	}

	constexpr float ToRadians(float degrees)
	{
		return degrees * 0.0174533f;
	}
}
