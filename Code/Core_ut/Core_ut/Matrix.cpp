#include <Catch2/catch.hpp>

#include <Core/Matrix.h>

TEST_CASE("Matrix4x4")
{
	SECTION("Matrix4x4::Zero")
	{
		Matrix4x4 matrix = Matrix4x4::Zero;
		CHECK(matrix.m_Column0 == Vector4f(0.f, 0.f, 0.f, 0.f));
		CHECK(matrix.m_Column1 == Vector4f(0.f, 0.f, 0.f, 0.f));
		CHECK(matrix.m_Column2 == Vector4f(0.f, 0.f, 0.f, 0.f));
		CHECK(matrix.m_Column3 == Vector4f(0.f, 0.f, 0.f, 0.f));
	}

	SECTION("Matrix4x4::Identity")
	{
		Matrix4x4 matrix = Matrix4x4::Identity;
		CHECK(matrix.m_Column0 == Vector4f(1.f, 0.f, 0.f, 0.f));
		CHECK(matrix.m_Column1 == Vector4f(0.f, 1.f, 0.f, 0.f));
		CHECK(matrix.m_Column2 == Vector4f(0.f, 0.f, 1.f, 0.f));
		CHECK(matrix.m_Column3 == Vector4f(0.f, 0.f, 0.f, 1.f));
	}

}