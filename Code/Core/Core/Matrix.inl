#pragma once

#include "Core/Vector.h"

constexpr Matrix4x4::Matrix4x4()
	: m_Data{}
{
}

constexpr Matrix4x4::Matrix4x4(
	const float v00, const float v01, const float v02, const float v03,
	const float v04, const float v05, const float v06, const float v07,
	const float v08, const float v09, const float v10, const float v11,
	const float v12, const float v13, const float v14, const float v15)
	: m_Data{ v00, v01, v02, v03, v04, v05, v06, v07, v08, v09, v10, v11, v12, v13, v14, v15 }
{
}

constexpr Matrix4x4::Matrix4x4(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector4f& v3)
	: m_Data{ v0.x, v0.y, v0.z, v0.w, v1.x, v1.y, v1.z, v1.w, v2.x, v2.y, v2.z, v2.w, v3.x, v3.y, v3.z, v3.w }
{
}

inline bool Matrix4x4::operator==(const Matrix4x4& rhs) const
{
	return m_Data[0] == rhs.m_Data[0]
		&& m_Data[1] == rhs.m_Data[1]
		&& m_Data[2] == rhs.m_Data[2]
		&& m_Data[3] == rhs.m_Data[3]

		&& m_Data[4] == rhs.m_Data[4]
		&& m_Data[5] == rhs.m_Data[5]
		&& m_Data[6] == rhs.m_Data[6]
		&& m_Data[7] == rhs.m_Data[7]

		&& m_Data[8] == rhs.m_Data[8]
		&& m_Data[9] == rhs.m_Data[9]
		&& m_Data[10] == rhs.m_Data[10]
		&& m_Data[11] == rhs.m_Data[11]

		&& m_Data[12] == rhs.m_Data[12]
		&& m_Data[13] == rhs.m_Data[13]
		&& m_Data[14] == rhs.m_Data[14]
		&& m_Data[15] == rhs.m_Data[15];
}

inline bool Matrix4x4::operator!=(const Matrix4x4& rhs) const
{
	return m_Data[0] != rhs.m_Data[0]
		|| m_Data[1] != rhs.m_Data[1]
		|| m_Data[2] != rhs.m_Data[2]
		|| m_Data[3] != rhs.m_Data[3]

		|| m_Data[4] != rhs.m_Data[4]
		|| m_Data[5] != rhs.m_Data[5]
		|| m_Data[6] != rhs.m_Data[6]
		|| m_Data[7] != rhs.m_Data[7]

		|| m_Data[8] != rhs.m_Data[8]
		|| m_Data[9] != rhs.m_Data[9]
		|| m_Data[10] != rhs.m_Data[10]
		|| m_Data[11] != rhs.m_Data[11]

		|| m_Data[12] != rhs.m_Data[12]
		|| m_Data[13] != rhs.m_Data[13]
		|| m_Data[14] != rhs.m_Data[14]
		|| m_Data[15] != rhs.m_Data[15];
}

inline Matrix4x4 Matrix4x4::operator+(const Matrix4x4& rhs) const
{
	return Matrix4x4{
		m_Data[0] + rhs.m_Data[0],
		m_Data[1] + rhs.m_Data[1],
		m_Data[2] + rhs.m_Data[2],
		m_Data[3] + rhs.m_Data[3],

		m_Data[4] + rhs.m_Data[4],
		m_Data[5] + rhs.m_Data[5],
		m_Data[6] + rhs.m_Data[6],
		m_Data[7] + rhs.m_Data[7],

		m_Data[8] + rhs.m_Data[8],
		m_Data[9] + rhs.m_Data[9],
		m_Data[10] + rhs.m_Data[10],
		m_Data[11] + rhs.m_Data[11],

		m_Data[12] + rhs.m_Data[12],
		m_Data[13] + rhs.m_Data[13],
		m_Data[14] + rhs.m_Data[14],
		m_Data[15] + rhs.m_Data[15] };
}

inline Matrix4x4 Matrix4x4::operator-(const Matrix4x4& rhs) const
{
	return Matrix4x4{
		m_Data[0] - rhs.m_Data[0],
		m_Data[1] - rhs.m_Data[1],
		m_Data[2] - rhs.m_Data[2],
		m_Data[3] - rhs.m_Data[3],

		m_Data[4] - rhs.m_Data[4],
		m_Data[5] - rhs.m_Data[5],
		m_Data[6] - rhs.m_Data[6],
		m_Data[7] - rhs.m_Data[7],

		m_Data[8] - rhs.m_Data[8],
		m_Data[9] - rhs.m_Data[9],
		m_Data[10] - rhs.m_Data[10],
		m_Data[11] - rhs.m_Data[11],

		m_Data[12] - rhs.m_Data[12],
		m_Data[13] - rhs.m_Data[13],
		m_Data[14] - rhs.m_Data[14],
		m_Data[15] - rhs.m_Data[15] };
}

inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const
{
	return Matrix4x4{
		m_Data[0] * rhs.m_Data[0] + m_Data[1] * rhs.m_Data[4] + m_Data[2] * rhs.m_Data[8] + m_Data[3] * rhs.m_Data[12],
		m_Data[0] * rhs.m_Data[1] + m_Data[1] * rhs.m_Data[5] + m_Data[2] * rhs.m_Data[9] + m_Data[3] * rhs.m_Data[13],
		m_Data[0] * rhs.m_Data[2] + m_Data[1] * rhs.m_Data[6] + m_Data[2] * rhs.m_Data[10] + m_Data[3] * rhs.m_Data[14],
		m_Data[0] * rhs.m_Data[3] + m_Data[1] * rhs.m_Data[7] + m_Data[2] * rhs.m_Data[11] + m_Data[3] * rhs.m_Data[15],

		m_Data[4] * rhs.m_Data[0] + m_Data[5] * rhs.m_Data[4] + m_Data[6] * rhs.m_Data[8] + m_Data[7] * rhs.m_Data[12],
		m_Data[4] * rhs.m_Data[1] + m_Data[5] * rhs.m_Data[5] + m_Data[6] * rhs.m_Data[9] + m_Data[7] * rhs.m_Data[13],
		m_Data[4] * rhs.m_Data[2] + m_Data[5] * rhs.m_Data[6] + m_Data[6] * rhs.m_Data[10] + m_Data[7] * rhs.m_Data[14],
		m_Data[4] * rhs.m_Data[3] + m_Data[5] * rhs.m_Data[7] + m_Data[6] * rhs.m_Data[11] + m_Data[7] * rhs.m_Data[15],

		m_Data[8] * rhs.m_Data[0] + m_Data[9] * rhs.m_Data[4] + m_Data[10] * rhs.m_Data[8] + m_Data[11] * rhs.m_Data[12],
		m_Data[8] * rhs.m_Data[1] + m_Data[9] * rhs.m_Data[5] + m_Data[10] * rhs.m_Data[9] + m_Data[11] * rhs.m_Data[13],
		m_Data[8] * rhs.m_Data[2] + m_Data[9] * rhs.m_Data[6] + m_Data[10] * rhs.m_Data[10] + m_Data[11] * rhs.m_Data[14],
		m_Data[8] * rhs.m_Data[3] + m_Data[9] * rhs.m_Data[7] + m_Data[10] * rhs.m_Data[11] + m_Data[11] * rhs.m_Data[15],

		m_Data[12] * rhs.m_Data[0] + m_Data[13] * rhs.m_Data[4] + m_Data[14] * rhs.m_Data[8] + m_Data[15] * rhs.m_Data[12],
		m_Data[12] * rhs.m_Data[1] + m_Data[13] * rhs.m_Data[5] + m_Data[14] * rhs.m_Data[9] + m_Data[15] * rhs.m_Data[13],
		m_Data[12] * rhs.m_Data[2] + m_Data[13] * rhs.m_Data[6] + m_Data[14] * rhs.m_Data[10] + m_Data[15] * rhs.m_Data[14],
		m_Data[12] * rhs.m_Data[3] + m_Data[13] * rhs.m_Data[7] + m_Data[14] * rhs.m_Data[11] + m_Data[15] * rhs.m_Data[15] };
}

inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs)
{
	m_Data[0] += rhs.m_Data[0];
	m_Data[1] += rhs.m_Data[1];
	m_Data[2] += rhs.m_Data[2];
	m_Data[3] += rhs.m_Data[3];

	m_Data[4] += rhs.m_Data[4];
	m_Data[5] += rhs.m_Data[5];
	m_Data[6] += rhs.m_Data[6];
	m_Data[7] += rhs.m_Data[7];

	m_Data[8] += rhs.m_Data[8];
	m_Data[9] += rhs.m_Data[9];
	m_Data[10] += rhs.m_Data[10];
	m_Data[11] += rhs.m_Data[11];

	m_Data[12] += rhs.m_Data[12];
	m_Data[13] += rhs.m_Data[13];
	m_Data[14] += rhs.m_Data[14];
	m_Data[15] += rhs.m_Data[15];

	return *this;
}

inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs)
{
	m_Data[0] -= rhs.m_Data[0];
	m_Data[1] -= rhs.m_Data[1];
	m_Data[2] -= rhs.m_Data[2];
	m_Data[3] -= rhs.m_Data[3];

	m_Data[4] -= rhs.m_Data[4];
	m_Data[5] -= rhs.m_Data[5];
	m_Data[6] -= rhs.m_Data[6];
	m_Data[7] -= rhs.m_Data[7];

	m_Data[8] -= rhs.m_Data[8];
	m_Data[9] -= rhs.m_Data[9];
	m_Data[10] -= rhs.m_Data[10];
	m_Data[11] -= rhs.m_Data[11];

	m_Data[12] -= rhs.m_Data[12];
	m_Data[13] -= rhs.m_Data[13];
	m_Data[14] -= rhs.m_Data[14];
	m_Data[15] -= rhs.m_Data[15];

	return *this;
}

inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	// #note: don't edit values line by line
	*this = Matrix4x4{
		m_Data[0] * rhs.m_Data[0] + m_Data[1] * rhs.m_Data[4] + m_Data[2] * rhs.m_Data[8] + m_Data[3] * rhs.m_Data[12],
		m_Data[0] * rhs.m_Data[1] + m_Data[1] * rhs.m_Data[5] + m_Data[2] * rhs.m_Data[9] + m_Data[3] * rhs.m_Data[13],
		m_Data[0] * rhs.m_Data[2] + m_Data[1] * rhs.m_Data[6] + m_Data[2] * rhs.m_Data[10] + m_Data[3] * rhs.m_Data[14],
		m_Data[0] * rhs.m_Data[3] + m_Data[1] * rhs.m_Data[7] + m_Data[2] * rhs.m_Data[11] + m_Data[3] * rhs.m_Data[15],

		m_Data[4] * rhs.m_Data[0] + m_Data[5] * rhs.m_Data[4] + m_Data[6] * rhs.m_Data[8] + m_Data[7] * rhs.m_Data[12],
		m_Data[4] * rhs.m_Data[1] + m_Data[5] * rhs.m_Data[5] + m_Data[6] * rhs.m_Data[9] + m_Data[7] * rhs.m_Data[13],
		m_Data[4] * rhs.m_Data[2] + m_Data[5] * rhs.m_Data[6] + m_Data[6] * rhs.m_Data[10] + m_Data[7] * rhs.m_Data[14],
		m_Data[4] * rhs.m_Data[3] + m_Data[5] * rhs.m_Data[7] + m_Data[6] * rhs.m_Data[11] + m_Data[7] * rhs.m_Data[15],

		m_Data[8] * rhs.m_Data[0] + m_Data[9] * rhs.m_Data[4] + m_Data[10] * rhs.m_Data[8] + m_Data[11] * rhs.m_Data[12],
		m_Data[8] * rhs.m_Data[1] + m_Data[9] * rhs.m_Data[5] + m_Data[10] * rhs.m_Data[9] + m_Data[11] * rhs.m_Data[13],
		m_Data[8] * rhs.m_Data[2] + m_Data[9] * rhs.m_Data[6] + m_Data[10] * rhs.m_Data[10] + m_Data[11] * rhs.m_Data[14],
		m_Data[8] * rhs.m_Data[3] + m_Data[9] * rhs.m_Data[7] + m_Data[10] * rhs.m_Data[11] + m_Data[11] * rhs.m_Data[15],

		m_Data[12] * rhs.m_Data[0] + m_Data[13] * rhs.m_Data[4] + m_Data[14] * rhs.m_Data[8] + m_Data[15] * rhs.m_Data[12],
		m_Data[12] * rhs.m_Data[1] + m_Data[13] * rhs.m_Data[5] + m_Data[14] * rhs.m_Data[9] + m_Data[15] * rhs.m_Data[13],
		m_Data[12] * rhs.m_Data[2] + m_Data[13] * rhs.m_Data[6] + m_Data[14] * rhs.m_Data[10] + m_Data[15] * rhs.m_Data[14],
		m_Data[12] * rhs.m_Data[3] + m_Data[13] * rhs.m_Data[7] + m_Data[14] * rhs.m_Data[11] + m_Data[15] * rhs.m_Data[15] };
	return *this;
}