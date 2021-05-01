#include "EnginePCH.h"
#include "Engine/VectorHelpers.h"

#include <cmath>

float math::Dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

float math::Length(const Vector2f& value)
{
	return std::sqrt(value.x * value.x + value.y * value.y);
}

float math::Length(const Vector3f& value)
{
	return std::sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
}

float math::LengthSqr(const Vector2f& value)
{
	return value.x * value.x + value.y * value.y;
}

float math::LengthSqr(const Vector3f& value)
{
	return value.x * value.x + value.y * value.y + value.z * value.z;
}

Vector2f math::Normalized(const Vector2f& value)
{
	//#todo: handle (0,0) vector
	return value / Length(value);
}

Vector2f math::Reflect(const Vector2f& vector, const Vector2f& normal)
{
	// -2 * (V dot N)*N + V
	const float dot2 = Dot(vector, normal) * -2.0f;
	return (dot2 * normal) + vector;
}

Vector2f math::Divide(const Vector2f& lhs, const Vector2f& rhs)
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

Vector2f math::Multiply(const Vector2f& lhs, const Vector2f& rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}
