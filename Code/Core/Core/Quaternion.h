#pragma once

class Vector3f;
class Vector4f;

/// \brief
class Quaternion
{
public:
	constexpr Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }
	constexpr explicit Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) { }
	constexpr explicit Quaternion(const Vector3f& value, const float w);
	constexpr explicit Quaternion(const Vector4f& value);

	bool operator==(const Quaternion& rhs) const { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w); }
	bool operator!=(const Quaternion& rhs) const { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w); }

	Quaternion operator+(const Quaternion& rhs) const { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Quaternion operator-(const Quaternion& rhs) const { return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	Quaternion operator*(const Quaternion& rhs) const;

	Quaternion& operator+=(const Quaternion& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	Quaternion& operator-=(const Quaternion& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
	Quaternion& operator*=(const Quaternion& rhs) { *this = *this * rhs; return *this; }

	Quaternion operator*(const float rhs) const { return Quaternion(x * rhs, y * rhs, z * rhs, w * rhs); }
	Quaternion operator/(const float rhs) const { return Quaternion(x / rhs, y / rhs, z / rhs, w / rhs); }

	Quaternion& operator*=(const float rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
	Quaternion& operator/=(const float rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

	Quaternion operator-() const { return Quaternion(-x, -y, -z, -w); }

	float Length() const;
	float LengthSqr() const { return x * x + y * y + z * z + w * w; }

	void Normalize();

	[[nodiscard]] Quaternion Normalized() const;

	static const Quaternion Identity;
	static const Quaternion Zero;

public:
	float x, y, z, w;
};

#include <Core/Quaternion.inl>