#include <Catch2/catch.hpp>

#include <Core/Quaternion.h>

TEST_CASE("Quaternion")
{
	SECTION("Quaternion()")
	{
		Quaternion quaternion;
		CHECK(quaternion.x == 0.f);
		CHECK(quaternion.y == 0.f);
		CHECK(quaternion.z == 0.f);
		CHECK(quaternion.w == 1.f);
	}

	SECTION("Quaternion(x, y, z, w)")
	{
		Quaternion quaternion(1.f, 2.f, 3.f, 4.f);
		CHECK(quaternion.x == 1.f);
		CHECK(quaternion.y == 2.f);
		CHECK(quaternion.z == 3.f);
		CHECK(quaternion.w == 4.f);
	}

	SECTION("operator==")
	{
		CHECK(Quaternion(0.f, 0.f, 0.f, 0.f) == Quaternion(0.f, 0.f, 0.f, 0.f));
		CHECK_FALSE(Quaternion(0.f, 0.f, 0.f, 0.f) == Quaternion(1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator!=")
	{
		CHECK(Quaternion(0.f, 0.f, 0.f, 0.f) != Quaternion(1.f, 1.f, 1.f, 1.f));
		CHECK_FALSE(Quaternion(0.f, 0.f, 0.f, 0.f) != Quaternion(0.f, 0.f, 0.f, 0.f));
	}
}
