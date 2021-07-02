#pragma once

#include <Core/Math.h>
#include <Core/Vectorf.h>

inline constexpr Quaternion::Quaternion(const Vector3f& value, const float w)
	: x(value.x), y(value.y), z(value.z), w(w)
{
}

inline constexpr Quaternion::Quaternion(const Vector4f& value)
	: x(value.x), y(value.y), z(value.z), w(value.w)
{
}

inline Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
	return Quaternion(
		((w * rhs.x + x * rhs.w) + y * rhs.z) - z * rhs.y,
		((w * rhs.y + y * rhs.w) + z * rhs.x) - x * rhs.z,
		((w * rhs.z + z * rhs.w) + x * rhs.y) - y * rhs.x,
		((w * rhs.w - x * rhs.x) - y * rhs.y) - z * rhs.z);
}

inline float Quaternion::Length() const
{
	return math::Sqrt(x * x + y * y + z * z + w * w);
}

inline void Quaternion::Normalize()
{
	*this *= 1.f / Length();
}

inline Quaternion Quaternion::Normalized() const
{
	Quaternion value(*this);
	value.Normalize();
	return value;
}
