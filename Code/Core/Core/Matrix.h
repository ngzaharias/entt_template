#pragma once

#include <Core/Vector.h>

/// \brief
class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(
		const float col0row0, const float col0row1, const float col0row2, const float col0row3,
		const float col1row0, const float col1row1, const float col1row2, const float col1row3,
		const float col2row0, const float col2row1, const float col2row2, const float col2row3,
		const float col3row0, const float col3row1, const float col3row2, const float col3row3);
	Matrix4x4(const Vector4f& col0, const Vector4f& col1, const Vector4f& col2, const Vector4f& col3);

	bool operator==(const Matrix4x4& rhs) const;
	bool operator!=(const Matrix4x4& rhs) const;

	Matrix4x4 operator+(const Matrix4x4& rhs) const;
	Matrix4x4 operator-(const Matrix4x4& rhs) const;

	Matrix4x4& operator+=(const Matrix4x4& rhs);
	Matrix4x4& operator-=(const Matrix4x4& rhs);

	static const Matrix4x4 Identity;
	static const Matrix4x4 Zero;

public:
	Vector4f m_Column0;
	Vector4f m_Column1;
	Vector4f m_Column2;
	Vector4f m_Column3;
};

#include <Core/Matrix.inl>