#include "Math.hpp"

inline float math::Sign(float value)
{
	return (value < 0.0f) ? -1.0f : 1.0f;
}

inline float math::ToDegrees(float radians)
{
	return radians * 57.2958f;
}

inline float math::ToRadians(float degrees)
{
	return degrees * 0.0174533f;
}
