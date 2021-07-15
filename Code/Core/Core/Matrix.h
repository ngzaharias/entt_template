#pragma once

#include <Core/Defines.h>

class Vector4f;

/// \brief
class ALIGN_16 Matrix4x4
{
public:
	constexpr Matrix4x4();
	constexpr Matrix4x4(
		const float v00, const float v01, const float v02, const float v03,
		const float v04, const float v05, const float v06, const float v07,
		const float v08, const float v09, const float v10, const float v11,
		const float v12, const float v13, const float v14, const float v15);
	constexpr Matrix4x4(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector4f& v3);

	bool operator==(const Matrix4x4& rhs) const;
	bool operator!=(const Matrix4x4& rhs) const;

	Matrix4x4 operator+(const Matrix4x4& rhs) const;
	Matrix4x4 operator-(const Matrix4x4& rhs) const;
	Matrix4x4 operator*(const Matrix4x4& rhs) const;

	Matrix4x4& operator+=(const Matrix4x4& rhs);
	Matrix4x4& operator-=(const Matrix4x4& rhs);
	Matrix4x4& operator*=(const Matrix4x4& rhs);

	static const Matrix4x4 Identity;
	static const Matrix4x4 Zero;

public:
	float m_Data[16];
};

#include <Core/Matrix.inl>