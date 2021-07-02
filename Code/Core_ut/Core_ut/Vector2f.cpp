#include <Catch2/catch.hpp>

#include <Core/Vectorf.h>

TEST_CASE("Vector2f")
{
	SECTION("Vector2f()")
	{
		Vector2f vector = Vector2f::Zero;
		CHECK(vector.x == 0.f);
		CHECK(vector.y == 0.f);
	}

	SECTION("Vector2f(value)")
	{
		Vector2f vector(1.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 1.f);
	}

	SECTION("Vector2f(x, y)")
	{
		Vector2f vector(1.f, 2.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
	}

	SECTION("operator==")
	{
		CHECK(Vector2f(0.f) == Vector2f(0.f));
		CHECK_FALSE(Vector2f(0.f) == Vector2f(1.f));
	}

	SECTION("operator!=")
	{
		CHECK(Vector2f(0.f) != Vector2f(1.f));
		CHECK_FALSE(Vector2f(0.f) != Vector2f(0.f));
	}

	SECTION("operator+")
	{
		Vector2f vector = Vector2f(1.f) + Vector2f(2.f);
		CHECK(vector == Vector2f(3.f));
	}

	SECTION("operator-")
	{
		Vector2f vector = Vector2f(3.f) - Vector2f(2.f);
		CHECK(vector == Vector2f(1.f));
	}

	SECTION("operator+=")
	{
		Vector2f vector = Vector2f(1.f);
		vector += Vector2f(2.f);
		CHECK(vector == Vector2f(3.f));
	}

	SECTION("operator-=")
	{
		Vector2f vector = Vector2f(3.f);
		vector -= Vector2f(2.f);
		CHECK(vector == Vector2f(1.f));
	}

	SECTION("operator*")
	{
		Vector2f vector = Vector2f(2.f) * 3.f;
		CHECK(vector == Vector2f(6.f));
	}

	SECTION("operator/")
	{
		Vector2f vector = Vector2f(6.f) / 3.f;
		CHECK(vector == Vector2f(2.f));
	}

	SECTION("operator*=")
	{
		Vector2f vector = Vector2f(2.f);
		vector *= 3.f;
		CHECK(vector == Vector2f(6.f));
	}

	SECTION("operator/=")
	{
		Vector2f vector = Vector2f(6.f);
		vector /= 3.f;
		CHECK(vector == Vector2f(2.f));
	}

	SECTION("Length")
	{
		CHECK(Vector2f(0.f, 0.f).Length() == 0.f);

		CHECK(Vector2f(1.f, 0.f).Length() == 1.f);
		CHECK(Vector2f(10.f, 0.f).Length() == 10.f);
		
		CHECK(Vector2f(-1.f, 0.f).Length() == 1.f);
		CHECK(Vector2f(-10.f, 0.f).Length() == 10.f);
	}

	SECTION("LengthSqr")
	{
		CHECK(Vector2f(0.f, 0.f).LengthSqr() == 0.f);
		CHECK(Vector2f(1.f, 0.f).LengthSqr() == 1.f);
		CHECK(Vector2f(10.f, 0.f).LengthSqr() == 100.f);
	}

	SECTION("Normalize")
	{
		Vector2f vector;

		vector = Vector2f(1.f, 0.f);
		vector.Normalize();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector2f(1.f, 0.f));

		vector = Vector2f(10.f, 0.f);
		vector.Normalize();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector2f(1.f, 0.f));
	}
	
	SECTION("NormalizeUnsafe")
	{
		Vector2f vector;

		vector = Vector2f(1.f, 0.f);
		vector.NormalizeUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector2f(1.f, 0.f));

		vector = Vector2f(10.f, 0.f);
		vector.NormalizeUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector2f(1.f, 0.f));
	}

	SECTION("Normalized")
	{
		Vector2f vector;

		vector = Vector2f(1.f, 0.f).Normalized();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector2f(1.f, 0.f));

		vector = Vector2f(10.f, 0.f).Normalized();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector2f(1.f, 0.f));
	}

	SECTION("NormalizedUnsafe")
	{
		Vector2f vector;

		vector = Vector2f(1.f, 0.f).NormalizedUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector2f(1.f, 0.f));

		vector = Vector2f(10.f, 0.f).NormalizedUnsafe();
		CHECK(vector.Length() == 1.f);
		CHECK(vector == Vector2f(1.f, 0.f));
	}

	SECTION("X0Y")
	{
		CHECK(Vector2f(1.f, 2.f).X0Y() == Vector3f(1.f, 0.f, 2.f));
	}
}
