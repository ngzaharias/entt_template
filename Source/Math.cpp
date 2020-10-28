#include "Math.hpp"

namespace Math
{
	float Sign(float value)
	{
		return (value < 0.0f) ? -1.0f : 1.0f;
	}
}