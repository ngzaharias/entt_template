#include <Catch2/catch.hpp>

#include <Engine/Algorithms.h>
#include <Engine/Set.h>
#include <Engine/Types.h>

TEST_CASE("Difference")
{
	SECTION("No Matches")
	{
		Set<int32> inputA = { 1, 2, 3 };
		Set<int32> inputB = { 1, 2, 3 };
		Set<int32> output = { };
		core::Difference(inputA, inputB, output);
		CHECK(output.empty());

		core::Difference(inputB, inputA, output);
		CHECK(output.empty());
	}

	SECTION("Match One from A")
	{
		Set<int32> inputA = { 1, 2, 3, 4 };
		Set<int32> inputB = { 1, 2, 3 };
		Set<int32> output = { };
		core::Difference(inputA, inputB, output);
		CHECK(output.size() == 1);
		CHECK(output.count(4) == 1);
	}

	SECTION("Match All from A")
	{
		Set<int32> inputA = { 1, 2, 3 };
		Set<int32> inputB = { 4, 5, 6 };
		Set<int32> output = { };
		core::Difference(inputA, inputB, output);
		CHECK(output.size() == 3);
		CHECK(output.count(1) == 1);
		CHECK(output.count(2) == 1);
		CHECK(output.count(3) == 1);
	}

	SECTION("Match All from A and B")
	{
		Set<int32> inputA = { 1, 2, 3 };
		Set<int32> inputB = { 4, 5, 6 };
		Set<int32> output = { };
		core::Difference(inputA, inputB, output);
		core::Difference(inputB, inputA, output);
		CHECK(output.size() == 6);
		CHECK(output.count(1) == 1);
		CHECK(output.count(2) == 1);
		CHECK(output.count(3) == 1);
		CHECK(output.count(4) == 1);
		CHECK(output.count(5) == 1);
		CHECK(output.count(6) == 1);
	}
}

TEST_CASE("Intersection")
{
	SECTION("No Matches")
	{
		Set<int32> inputA = { 1, 2, 3 };
		Set<int32> inputB = { 4, 5, 6 };
		Set<int32> output = { };
		core::Intersection(inputA, inputB, output);
		CHECK(output.empty());
	}

	SECTION("Match One")
	{
		Set<int32> inputA = { 1, 2, 3 };
		Set<int32> inputB = { 3, 4, 5 };
		Set<int32> output = { };
		core::Intersection(inputA, inputB, output);
		CHECK(output.size() == 1);
		CHECK(output.count(3) == 1);
	}

	SECTION("Match All")
	{
		Set<int32> inputA = { 1, 2, 3 };
		Set<int32> inputB = { 1, 2, 3 };
		Set<int32> output = { };
		core::Intersection(inputA, inputB, output);
		CHECK(output.size() == 3);
		CHECK(output.count(1) == 1);
		CHECK(output.count(2) == 1);
		CHECK(output.count(3) == 1);
	}
}