#pragma once

class Vector2f;
class Vector3f;
class Vector4f;

/// \brief
class Vector2f
{
public:
	constexpr Vector2f() : x(), y() {}
	constexpr explicit Vector2f(const float value) : x(value), y(value) {}
	constexpr explicit Vector2f(const float x, const float y) : x(x), y(y) {}

	bool operator==(const Vector2f& rhs) const { return (x == rhs.x) && (y == rhs.y); }
	bool operator!=(const Vector2f& rhs) const { return (x != rhs.x) || (y != rhs.y); }

	Vector2f operator+(const Vector2f& rhs) const { return Vector2f(x + rhs.x, y + rhs.y); }
	Vector2f operator-(const Vector2f& rhs) const { return Vector2f(x - rhs.x, y - rhs.y); }

	Vector2f& operator+=(const Vector2f& rhs) { x += rhs.x; y += rhs.y; return *this; }
	Vector2f& operator-=(const Vector2f& rhs) { x -= rhs.x; y -= rhs.y; return *this; }

	Vector2f operator*(const float rhs) const { return Vector2f(x * rhs, y * rhs); }
	Vector2f operator/(const float rhs) const { return Vector2f(x / rhs, y / rhs); }

	Vector2f& operator*=(const float rhs) { x *= rhs; y *= rhs; return *this; }
	Vector2f& operator/=(const float rhs) { x /= rhs; y /= rhs; return *this; }
	Vector2f operator-() const { return Vector2f(-x, -y); }

	float Length() const;
	float LengthSqr() const { return x * x + y * y; }

	void Normalize();
	void NormalizeUnsafe();

	[[nodiscard]] Vector2f Normalized() const;
	[[nodiscard]] Vector2f NormalizedUnsafe() const;

	Vector3f X0Y() const;

	static const Vector2f Zero;

public:
	float x, y;
};

/// \brief
class Vector3f
{
public:
	constexpr Vector3f() : x(), y(), z() {}
	constexpr explicit Vector3f(const float value) : x(value), y(value), z(value) {}
	constexpr explicit Vector3f(const float x, const float y, const float z) : x(x), y(y), z(z) {}

	bool operator==(const Vector3f& rhs) const { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z); }
	bool operator!=(const Vector3f& rhs) const { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z); }

	Vector3f operator+(const Vector3f& rhs) const { return Vector3f(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3f operator-(const Vector3f& rhs) const { return Vector3f(x - rhs.x, y - rhs.y, z - rhs.z); }

	Vector3f& operator+=(const Vector3f& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	Vector3f& operator-=(const Vector3f& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

	Vector3f operator*(const float rhs) const { return Vector3f(x * rhs, y * rhs, z * rhs); }
	Vector3f operator/(const float rhs) const { return Vector3f(x / rhs, y / rhs, z / rhs); }

	Vector3f& operator*=(const float rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
	Vector3f& operator/=(const float rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }

	Vector3f operator-() const { return Vector3f(-x, -y, -z); }

	float Length() const;
	float LengthSqr() const { return x * x + y * y + z * z; }

	void Normalize();
	void NormalizeUnsafe();

	[[nodiscard]] Vector3f Normalized() const;
	[[nodiscard]] Vector3f NormalizedUnsafe() const;

	Vector2f XY() const;
	Vector2f XZ() const;
	Vector3f X0Z() const;

	static const Vector3f Zero;

public:
	float x, y, z;
};

/// \brief
class Vector4f
{
public:
	constexpr Vector4f() : x(), y(), z(), w() { }
	constexpr explicit Vector4f(const Vector3f& value, const float w) : x(value.x), y(value.y), z(value.z), w(w) { }
	constexpr explicit Vector4f(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) { }

	bool operator==(const Vector4f& rhs) const { return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w); }
	bool operator!=(const Vector4f& rhs) const { return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w); }

	Vector4f operator+(const Vector4f& rhs) const { return Vector4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Vector4f operator-(const Vector4f& rhs) const { return Vector4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }

	Vector4f& operator+=(const Vector4f& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	Vector4f& operator-=(const Vector4f& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }

	Vector4f operator*(const float rhs) const { return Vector4f(x * rhs, y * rhs, z * rhs, w * rhs); }
	Vector4f operator/(const float rhs) const { return Vector4f(x / rhs, y / rhs, z / rhs, w / rhs); }

	Vector4f& operator*=(const float rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
	Vector4f& operator/=(const float rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

	Vector4f operator-() const { return Vector4f(-x, -y, -z, -w); }

	float Length() const;
	float LengthSqr() const { return x * x + y * y + z * z + w * w; }

	void Normalize();
	void NormalizeUnsafe();

	[[nodiscard]] Vector4f Normalized() const;
	[[nodiscard]] Vector4f NormalizedUnsafe() const;

	static const Vector4f Identity;
	static const Vector4f Zero;

public:
	float x, y, z, w;
};

#include <Core/Vectorf.inl>