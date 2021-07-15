#include <Catch2/catch.hpp>

#include <Core/Vector.h>

TEST_CASE("Vector4f")
{
	SECTION("Alignment")
	{
		CHECK(alignof(Vector4f) == 16);
	}

	SECTION("Vector4f::Zero")
	{
		Vector4f vector = Vector4f::Zero;
		CHECK(vector.x == 0.f);
		CHECK(vector.y == 0.f);
		CHECK(vector.z == 0.f);
		CHECK(vector.w == 0.f);
	}

	SECTION("Vector4f::Identity")
	{
		Vector4f vector = Vector4f::Identity;
		CHECK(vector.x == 0.f);
		CHECK(vector.y == 0.f);
		CHECK(vector.z == 0.f);
		CHECK(vector.w == 1.f);
	}

	SECTION("Vector4f(Vector3f, w)")
	{
		Vector4f vector(Vector3f(1.f, 2.f, 3.f), 4.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
		CHECK(vector.w == 4.f);
	}

	SECTION("Vector4f(x, y, z, w)")
	{
		Vector4f vector(1.f, 2.f, 3.f, 4.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
		CHECK(vector.w == 4.f);
	}

	SECTION("operator==")
	{
		CHECK(Vector4f(0.f, 0.f, 0.f, 0.f) == Vector4f(0.f, 0.f, 0.f, 0.f));
		CHECK_FALSE(Vector4f(0.f, 0.f, 0.f, 0.f) == Vector4f(1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator!=")
	{
		CHECK(Vector4f(0.f, 0.f, 0.f, 0.f) != Vector4f(1.f, 1.f, 1.f, 1.f));
		CHECK_FALSE(Vector4f(0.f, 0.f, 0.f, 0.f) != Vector4f(0.f, 0.f, 0.f, 0.f));
	}

	SECTION("operator+")
	{
		Vector4f vector = Vector4f(1.f, 1.f, 1.f, 1.f) + Vector4f(2.f, 2.f, 2.f, 2.f);
		CHECK(vector == Vector4f(3.f, 3.f, 3.f, 3.f));
	}

	SECTION("operator-")
	{
		Vector4f vector = Vector4f(3.f, 3.f, 3.f, 3.f) - Vector4f(2.f, 2.f, 2.f, 2.f);
		CHECK(vector == Vector4f(1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator+=")
	{
		Vector4f vector = Vector4f(1.f, 1.f, 1.f, 1.f);
		vector += Vector4f(2.f, 2.f, 2.f, 2.f);
		CHECK(vector == Vector4f(3.f, 3.f, 3.f, 3.f));
	}

	SECTION("operator-=")
	{
		Vector4f vector = Vector4f(3.f, 3.f, 3.f, 3.f);
		vector -= Vector4f(2.f, 2.f, 2.f, 2.f);
		CHECK(vector == Vector4f(1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator*")
	{
		Vector4f vector = Vector4f(2.f, 2.f, 2.f, 2.f) * 3.f;
		CHECK(vector == Vector4f(6.f, 6.f, 6.f, 6.f));
	}

	SECTION("operator/")
	{
		Vector4f vector = Vector4f(6.f, 6.f, 6.f, 6.f) / 3.f;
		CHECK(vector == Vector4f(2.f, 2.f, 2.f, 2.f));
	}

	SECTION("operator*=")
	{
		Vector4f vector = Vector4f(2.f, 2.f, 2.f, 2.f);
		vector *= 3.f;
		CHECK(vector == Vector4f(6.f, 6.f, 6.f, 6.f));
	}

	SECTION("operator/=")
	{
		Vector4f vector = Vector4f(6.f, 6.f, 6.f, 6.f);
		vector /= 3.f;
		CHECK(vector == Vector4f(2.f, 2.f, 2.f, 2.f));
	}

	SECTION("Length")
	{
		CHECK(Vector4f(0.f, 0.f, 0.f, 0.f).Length() == 0.f);

		CHECK(Vector4f(1.f, 0.f, 0.f, 0.f).Length() == 1.f);
		CHECK(Vector4f(10.f, 0.f, 0.f, 0.f).Length() == 10.f);

		CHECK(Vector4f(-1.f, 0.f, 0.f, 0.f).Length() == 1.f);
		CHECK(Vector4f(-10.f, 0.f, 0.f, 0.f).Length() == 10.f);
	}

	SECTION("LengthSqr")
	{
		CHECK(Vector4f(0.f, 0.f, 0.f, 0.f).LengthSqr() == 0.f);
		CHECK(Vector4f(1.f, 0.f, 0.f, 0.f).LengthSqr() == 1.f);
		CHECK(Vector4f(10.f, 0.f, 0.f, 0.f).LengthSqr() == 100.f);
	}

	SECTION("Normalize")
	{
		Vector4f vector;

		vector = Vector4f(1.f, 0.f, 0.f, 0.f);
		vector.Normalize();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector4f(1.f, 0.f, 0.f, 0.f));

		vector = Vector4f(10.f, 0.f, 0.f, 0.f);
		vector.Normalize();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector4f(1.f, 0.f, 0.f, 0.f));
	}

	SECTION("NormalizeUnsafe")
	{
		Vector4f vector;

		vector = Vector4f(1.f, 0.f, 0.f, 0.f);
		vector.NormalizeUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector4f(1.f, 0.f, 0.f, 0.f));

		vector = Vector4f(10.f, 0.f, 0.f, 0.f);
		vector.NormalizeUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector4f(1.f, 0.f, 0.f, 0.f));
	}

	SECTION("Normalized")
	{
		Vector4f vector;

		vector = Vector4f(1.f, 0.f, 0.f, 0.f).Normalized();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector4f(1.f, 0.f, 0.f, 0.f));

		vector = Vector4f(10.f, 0.f, 0.f, 0.f).Normalized();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector4f(1.f, 0.f, 0.f, 0.f));
	}

	SECTION("NormalizedUnsafe")
	{
		Vector4f vector;

		vector = Vector4f(1.f, 0.f, 0.f, 0.f).NormalizedUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector4f(1.f, 0.f, 0.f, 0.f));

		vector = Vector4f(10.f, 0.f, 0.f, 0.f).NormalizedUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector4f(1.f, 0.f, 0.f, 0.f));
	}
}
