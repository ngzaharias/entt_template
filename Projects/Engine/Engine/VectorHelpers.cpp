#include "Engine/EnginePCH.h"
#include "Engine/VectorHelpers.h"

#include <cmath>

float Dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

float Length(const Vector2f& value)
{
	return std::sqrt(value.x * value.x + value.y * value.y);
}

Vector2f Normalized(const Vector2f& value)
{
	//#todo: handle (0,0) vector
	return value / Length(value);
}

Vector2f Reflect(const Vector2f& vector, const Vector2f& normal)
{
	// -2 * (V dot N)*N + V
	const float dot2 = Dot(vector, normal) * -2.0f;
	return (dot2 * normal) + vector;
}

Vector2f Divide(const Vector2f& lhs, const Vector2f& rhs)
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

Vector2f Multiply(const Vector2f& lhs, const Vector2f& rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}
