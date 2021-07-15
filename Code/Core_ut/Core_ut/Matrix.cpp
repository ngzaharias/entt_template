#include <Catch2/catch.hpp>

#include <Core/Matrix.h>

TEST_CASE("Matrix4x4")
{
	SECTION("Alignment")
	{
		CHECK(alignof(Matrix4x4) == 16);
	}

	SECTION("Matrix4x4::Zero")
	{
		CHECK(Matrix4x4::Zero == Matrix4x4(
			0.f, 0.f, 0.f, 0.f, 
			0.f, 0.f, 0.f, 0.f, 
			0.f, 0.f, 0.f, 0.f, 
			0.f, 0.f, 0.f, 0.f));
	}

	SECTION("Matrix4x4::Identity")
	{
		CHECK(Matrix4x4::Identity == Matrix4x4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f));
	}

	SECTION("Matrix4x4(v00, v01, v02, v03, v04, v05, v06, v07, v08, v09, v10, v11, v12, v13, v14, v15)")
	{
		const Matrix4x4 matrix(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f);
		CHECK(matrix.m_Data[0] == 1.f);
		CHECK(matrix.m_Data[1] == 2.f);
		CHECK(matrix.m_Data[2] == 3.f);
		CHECK(matrix.m_Data[3] == 4.f);

		CHECK(matrix.m_Data[4] == 5.f);
		CHECK(matrix.m_Data[5] == 6.f);
		CHECK(matrix.m_Data[6] == 7.f);
		CHECK(matrix.m_Data[7] == 8.f);

		CHECK(matrix.m_Data[8] == 9.f);
		CHECK(matrix.m_Data[9] == 10.f);
		CHECK(matrix.m_Data[10] == 11.f);
		CHECK(matrix.m_Data[11] == 12.f);

		CHECK(matrix.m_Data[12] == 13.f);
		CHECK(matrix.m_Data[13] == 14.f);
		CHECK(matrix.m_Data[14] == 15.f);
		CHECK(matrix.m_Data[15] == 16.f);
	}

	SECTION("Matrix4x4(v0, v1, v2, v3)")
	{
		const Matrix4x4 matrix(
			{ 01.f, 02.f, 03.f, 04.f },
			{ 05.f, 06.f, 07.f, 08.f },
			{ 09.f, 10.f, 11.f, 12.f },
			{ 13.f, 14.f, 15.f, 16.f });

		CHECK(matrix.m_Data[0] == 01.f);
		CHECK(matrix.m_Data[1] == 02.f);
		CHECK(matrix.m_Data[2] == 03.f);
		CHECK(matrix.m_Data[3] == 04.f);

		CHECK(matrix.m_Data[4] == 05.f);
		CHECK(matrix.m_Data[5] == 06.f);
		CHECK(matrix.m_Data[6] == 07.f);
		CHECK(matrix.m_Data[7] == 08.f);

		CHECK(matrix.m_Data[8] == 09.f);
		CHECK(matrix.m_Data[9] == 10.f);
		CHECK(matrix.m_Data[10] == 11.f);
		CHECK(matrix.m_Data[11] == 12.f);

		CHECK(matrix.m_Data[12] == 13.f);
		CHECK(matrix.m_Data[13] == 14.f);
		CHECK(matrix.m_Data[14] == 15.f);
		CHECK(matrix.m_Data[15] == 16.f);
	}

	SECTION("operator==")
	{
		CHECK(Matrix4x4::Zero == Matrix4x4::Zero);
		CHECK_FALSE(Matrix4x4::Zero == Matrix4x4::Identity);
	}

	SECTION("operator!=")
	{
		CHECK(Matrix4x4::Zero != Matrix4x4::Identity);
		CHECK_FALSE(Matrix4x4::Zero != Matrix4x4::Zero);
	}

	SECTION("operator+")
	{
		const Matrix4x4 matrix = 
			Matrix4x4(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f) +
			Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f));
	}

	SECTION("operator-")
	{
		const Matrix4x4 matrix = 
			Matrix4x4(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f) -
			Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator*")
	{
		const Matrix4x4 matrix =
			Matrix4x4(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f) *
			Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f));
	}

	SECTION("operator+=")
	{
		Matrix4x4 matrix = Matrix4x4(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
		matrix += Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f));
	}

	SECTION("operator-=")
	{
		Matrix4x4 matrix = Matrix4x4(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f);
		matrix -= Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator*=")
	{
		Matrix4x4 matrix = Matrix4x4(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f);
		matrix *= Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f));
	}
}