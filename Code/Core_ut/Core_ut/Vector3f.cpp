#include <Catch2/catch.hpp>

#include <Core/Vector.h>

TEST_CASE("Vector3f")
{
	SECTION("Vector3f::Zero")
	{
		Vector3f vector = Vector3f::Zero;
		CHECK(vector.x == 0.f);
		CHECK(vector.y == 0.f);
		CHECK(vector.z == 0.f);
	}

	SECTION("Vector3f(value)")
	{
		Vector3f vector(1.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 1.f);
		CHECK(vector.z == 1.f);
	}

	SECTION("Vector3f(x, y, z)")
	{
		Vector3f vector(1.f, 2.f, 3.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
	}

	SECTION("operator==")
	{
		CHECK(Vector3f(0.f) == Vector3f(0.f));
		CHECK_FALSE(Vector3f(0.f) == Vector3f(1.f));
	}

	SECTION("operator!=")
	{
		CHECK(Vector3f(0.f) != Vector3f(1.f));
		CHECK_FALSE(Vector3f(0.f) != Vector3f(0.f));
	}

	SECTION("operator+")
	{
		Vector3f vector = Vector3f(1.f) + Vector3f(2.f);
		CHECK(vector == Vector3f(3.f));
	}

	SECTION("operator-")
	{
		Vector3f vector = Vector3f(3.f) - Vector3f(2.f);
		CHECK(vector == Vector3f(1.f));
	}

	SECTION("operator+=")
	{
		Vector3f vector = Vector3f(1.f);
		vector += Vector3f(2.f);
		CHECK(vector == Vector3f(3.f));
	}

	SECTION("operator-=")
	{
		Vector3f vector = Vector3f(3.f);
		vector -= Vector3f(2.f);
		CHECK(vector == Vector3f(1.f));
	}

	SECTION("operator*")
	{
		Vector3f vector = Vector3f(2.f) * 3.f;
		CHECK(vector == Vector3f(6.f));
	}

	SECTION("operator/")
	{
		Vector3f vector = Vector3f(6.f) / 3.f;
		CHECK(vector == Vector3f(2.f));
	}

	SECTION("operator*=")
	{
		Vector3f vector = Vector3f(2.f);
		vector *= 3.f;
		CHECK(vector == Vector3f(6.f));
	}

	SECTION("operator/=")
	{
		Vector3f vector = Vector3f(6.f);
		vector /= 3.f;
		CHECK(vector == Vector3f(2.f));
	}

	SECTION("Length")
	{
		CHECK(Vector3f(0.f, 0.f, 0.f).Length() == 0.f);

		CHECK(Vector3f(1.f, 0.f, 0.f).Length() == 1.f);
		CHECK(Vector3f(10.f, 0.f, 0.f).Length() == 10.f);

		CHECK(Vector3f(-1.f, 0.f, 0.f).Length() == 1.f);
		CHECK(Vector3f(-10.f, 0.f, 0.f).Length() == 10.f);
	}

	SECTION("LengthSqr")
	{
		CHECK(Vector3f(0.f, 0.f, 0.f).LengthSqr() == 0.f);
		CHECK(Vector3f(1.f, 0.f, 0.f).LengthSqr() == 1.f);
		CHECK(Vector3f(10.f, 0.f, 0.f).LengthSqr() == 100.f);
	}

	SECTION("Normalize")
	{
		Vector3f vector;

		vector = Vector3f(1.f, 0.f, 0.f);
		vector.Normalize();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector3f(1.f, 0.f, 0.f));

		vector = Vector3f(10.f, 0.f, 0.f);
		vector.Normalize();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector3f(1.f, 0.f, 0.f));
	}

	SECTION("NormalizeUnsafe")
	{
		Vector3f vector;

		vector = Vector3f(1.f, 0.f, 0.f);
		vector.NormalizeUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector3f(1.f, 0.f, 0.f));

		vector = Vector3f(10.f, 0.f, 0.f);
		vector.NormalizeUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector3f(1.f, 0.f, 0.f));
	}

	SECTION("Normalized")
	{
		Vector3f vector;

		vector = Vector3f(1.f, 0.f, 0.f).Normalized();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector3f(1.f, 0.f, 0.f));

		vector = Vector3f(10.f, 0.f, 0.f).Normalized();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector3f(1.f, 0.f, 0.f));
	}

	SECTION("NormalizedUnsafe")
	{
		Vector3f vector;

		vector = Vector3f(1.f, 0.f, 0.f).NormalizedUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector3f(1.f, 0.f, 0.f));

		vector = Vector3f(10.f, 0.f, 0.f).NormalizedUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector3f(1.f, 0.f, 0.f));
	}

	SECTION("XY")
	{
		CHECK(Vector3f(1.f, 2.f, 3.f).XY() == Vector2f(1.f, 2.f));
	}

	SECTION("XZ")
	{
		CHECK(Vector3f(1.f, 2.f, 3.f).XZ() == Vector2f(1.f, 3.f));
	}

	SECTION("X0Z")
	{
		CHECK(Vector3f(1.f, 2.f, 3.f).X0Z() == Vector3f(1.f, 0.f, 3.f));
	}
}
