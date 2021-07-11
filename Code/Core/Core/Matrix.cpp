#include "Core/Matrix.h"

const Matrix4x4 Matrix4x4::Identity({
	{ 1.f, 0.f, 0.f, 0.f },
	{ 0.f, 1.f, 0.f, 0.f },
	{ 0.f, 0.f, 1.f, 0.f },
	{ 0.f, 0.f, 0.f, 1.f } });
const Matrix4x4 Matrix4x4::Zero({
	{ 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f } });

Matrix4x4::Matrix4x4()
	: m_Column0()
	, m_Column1()
	, m_Column2()
	, m_Column3()
{
}

Matrix4x4::Matrix4x4(
	const float col0row0, const float col0row1, const float col0row2, const float col0row3,
	const float col1row0, const float col1row1, const float col1row2, const float col1row3,
	const float col2row0, const float col2row1, const float col2row2, const float col2row3,
	const float col3row0, const float col3row1, const float col3row2, const float col3row3)
	: m_Column0({ col0row0, col0row1, col0row2, col0row3 })
	, m_Column1({ col1row0, col1row1, col1row2, col1row3 })
	, m_Column2({ col2row0, col2row1, col2row2, col2row3 })
	, m_Column3({ col3row0, col3row1, col3row2, col3row3 })
{
}

 Matrix4x4::Matrix4x4(const Vector4f& col0, const Vector4f& col1, const Vector4f& col2, const Vector4f& col3)
	: m_Column0(col0)
	, m_Column1(col1)
	, m_Column2(col2)
	, m_Column3(col3)
{
}

bool Matrix4x4::operator==(const Matrix4x4& rhs) const
{
	return m_Column0 == rhs.m_Column0
		&& m_Column1 == rhs.m_Column1
		&& m_Column2 == rhs.m_Column2
		&& m_Column3 == rhs.m_Column3;
}

bool Matrix4x4::operator!=(const Matrix4x4& rhs) const
{
	return m_Column0 != rhs.m_Column0
		|| m_Column1 != rhs.m_Column1
		|| m_Column2 != rhs.m_Column2
		|| m_Column3 != rhs.m_Column3;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& rhs) const
{
	return Matrix4x4 {
		m_Column0 + rhs.m_Column0,
		m_Column1 + rhs.m_Column1,
		m_Column2 + rhs.m_Column2,
		m_Column3 + rhs.m_Column3 };
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& rhs) const
{
	return Matrix4x4{
		m_Column0 - rhs.m_Column0,
		m_Column1 - rhs.m_Column1,
		m_Column2 - rhs.m_Column2,
		m_Column3 - rhs.m_Column3 };
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs)
{
	m_Column0 += rhs.m_Column0;
	m_Column1 += rhs.m_Column1;
	m_Column2 += rhs.m_Column2;
	m_Column3 += rhs.m_Column3;
	return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs)
{
	m_Column0 -= rhs.m_Column0;
	m_Column1 -= rhs.m_Column1;
	m_Column2 -= rhs.m_Column2;
	m_Column3 -= rhs.m_Column3;
	return *this;
}
