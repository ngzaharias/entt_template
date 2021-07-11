#include <Catch2/catch.hpp>

#include <Core/Vector.h>

#include <Engine/Array.h>
#include <Engine/Map.h>
#include <Engine/Name.h>
#include <Engine/Nullable.h>
#include <Engine/Set.h>
#include <Engine/String.h>
#include <Engine/StringView.h>
#include <Engine/Variant.h>
#include <Engine/VariantHelpers.h>

#include <refl/refl.hpp>

// include last
#include <Engine/Reader.h>
#include <Engine/Writer.h>

namespace
{
	constexpr bool s_Bool = true;
	constexpr int32 s_Int32 = -1337;
	constexpr uint32 s_Uint32 = 666;
	constexpr double s_Double = 1.11111;
	constexpr float s_Float = 1.2345f;

	const str::Name s_Name = NAME("Nicholas");
	const str::String s_String = "Hello";
	const Vector2f s_Vector2f = Vector2f(1.f, 2.f);
	const Vector2i s_Vector2i = Vector2i(-1, -2);
	const Vector2u s_Vector2u = Vector2u(1, 2);
	const Vector3f s_Vector3f = Vector3f(1.f, 2.f, 3.f);
	const Vector3i s_Vector3i = Vector3i(-1, -2, -3);
}

TEST_CASE("Trivial")
{
	str::String writeString;

	{
		serialize::Writer writer;
		writer.Visit(s_Bool);
		writer.Visit(s_Int32);
		writer.Visit(s_Uint32);
		writer.Visit(s_Double);
		writer.Visit(s_Float);
		writer.Visit(s_Name);
		writer.Visit(s_String);
		writer.Visit(s_Vector2f);
		writer.Visit(s_Vector2i);
		writer.Visit(s_Vector2u);
		writer.Visit(s_Vector3f);
		writer.Visit(s_Vector3i);

		writeString = writer.Conclude();
	}

	{
		bool myBool; int32 myInt; uint32 myUint;
		double myDouble; float myFloat;
		str::Name myName; str::String myString;
		Vector2f myVector2f; Vector2i myVector2i; Vector2u myVector2u;
		Vector3f myVector3f; Vector3i myVector3i;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myBool);
		reader.Visit(myInt);
		reader.Visit(myUint);
		reader.Visit(myDouble);
		reader.Visit(myFloat);
		reader.Visit(myName);
		reader.Visit(myString);
		reader.Visit(myVector2f);
		reader.Visit(myVector2i);
		reader.Visit(myVector2u);
		reader.Visit(myVector3f);
		reader.Visit(myVector3i);

		REQUIRE(myBool == s_Bool);
		REQUIRE(myInt == s_Int32);
		REQUIRE(myUint == s_Uint32);
		REQUIRE(myDouble == s_Double);
		REQUIRE(myFloat == s_Float);
		REQUIRE(myName == s_Name);
		REQUIRE(myString == s_String);
		REQUIRE(myVector2f == s_Vector2f);
		REQUIRE(myVector2i == s_Vector2i);
		REQUIRE(myVector2u == s_Vector2u);
		REQUIRE(myVector3f == s_Vector3f);
		REQUIRE(myVector3i == s_Vector3i);
	}
}

struct MyStruct
{
	bool myBool;
	int32 myInt32;
	uint32 myUint32;
	double myDouble;
	float myFloat;
	str::Name myName;
	str::String myString;
	Vector2f myVector2f;
	Vector2i myVector2i;
	Vector2u myVector2u;
	Vector3f myVector3f;
	Vector3i myVector3i;
};

REFL_AUTO
(
	type(MyStruct)
	, field(myBool)
	, field(myInt32)
	, field(myUint32)
	, field(myDouble)
	, field(myFloat)
	, field(myName)
	, field(myString)
	, field(myVector2f)
	, field(myVector2i)
	, field(myVector2u)
	, field(myVector3f)
	, field(myVector3i)
)

TEST_CASE("Struct")
{
	str::String writeString;

	{
		MyStruct myStruct = 
		{ 
			s_Bool
			, s_Int32
			, s_Uint32
			, s_Double
			, s_Float 
			, s_Name
			, s_String
			, s_Vector2f
			, s_Vector2i
			, s_Vector2u
			, s_Vector3f
			, s_Vector3i
		};

		serialize::Writer writer;
		writer.Visit(myStruct);

		writeString = writer.Conclude();
	}

	{
		MyStruct myStruct;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myStruct);

		REQUIRE(myStruct.myBool == s_Bool);
		REQUIRE(myStruct.myInt32 == s_Int32);
		REQUIRE(myStruct.myUint32 == s_Uint32);
		REQUIRE(myStruct.myDouble == s_Double);
		REQUIRE(myStruct.myFloat == s_Float);
		REQUIRE(myStruct.myName == s_Name);
		REQUIRE(myStruct.myString == s_String);
		REQUIRE(myStruct.myVector2f == s_Vector2f);
		REQUIRE(myStruct.myVector2i == s_Vector2i);
		REQUIRE(myStruct.myVector2u == s_Vector2u);
		REQUIRE(myStruct.myVector3f == s_Vector3f);
		REQUIRE(myStruct.myVector3i == s_Vector3i);
	}
}

TEST_CASE("Array")
{
	str::String writeString;

	{
		Array<int32> myVector = { 1, 2, 3, 4, 5 };

		serialize::Writer writer;
		writer.Visit(myVector);

		writeString = writer.Conclude();
	}

	{
		Array<int32> myVector;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myVector);

		REQUIRE(myVector.size() == 5);
		REQUIRE(myVector[0] == 1);
		REQUIRE(myVector[1] == 2);
		REQUIRE(myVector[2] == 3);
		REQUIRE(myVector[3] == 4);
		REQUIRE(myVector[4] == 5);
	}
}

TEST_CASE("Map")
{
	str::String writeString;

	{
		Map<int32, bool> myMap = { {3, true}, {2, false}, {1, true} };

		serialize::Writer writer;
		writer.Visit(myMap);

		writeString = writer.Conclude();
	}

	{
		Map<int32, bool> myMap;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myMap);

		REQUIRE(myMap.size() == 3);
		REQUIRE(myMap.count(1) == 1);
		REQUIRE(myMap.count(2) == 1);
		REQUIRE(myMap.count(3) == 1);
	}
}

TEST_CASE("Set")
{
	str::String writeString;

	{
		std::set<int32> mySet = { 5, 4, 3, 2, 1 };

		serialize::Writer writer;
		writer.Visit(mySet);

		writeString = writer.Conclude();
	}

	{
		Array<int32> mySet;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(mySet);

		REQUIRE(mySet.size() == 5);
		REQUIRE(mySet[0] == 1);
		REQUIRE(mySet[1] == 2);
		REQUIRE(mySet[2] == 3);
		REQUIRE(mySet[3] == 4);
		REQUIRE(mySet[4] == 5);
	}
}

TEST_CASE("Nullable")
{
	using Nullable = Nullable<int32>;
	str::String writeString;

	{
		Nullable myNullableA = s_Int32;
		Nullable myNullableB = { };

		serialize::Writer writer;
		writer.Visit(myNullableA);
		writer.Visit(myNullableB);

		writeString = writer.Conclude();
	}

	{
		Nullable myNullableA, myNullableB;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myNullableA);
		reader.Visit(myNullableB);

		REQUIRE(myNullableA.has_value());
		REQUIRE(myNullableA.value() == s_Int32);
		REQUIRE(!myNullableB.has_value());
	}
}

TEST_CASE("Variant")
{
	using MyVariant = Variant<bool, int32, float>;
	str::String writeString;

	{
		MyVariant myVariantA = s_Bool;
		MyVariant myVariantB = s_Int32;
		MyVariant myVariantC = s_Float;

		serialize::Writer writer;
		writer.Visit(myVariantA);
		writer.Visit(myVariantB);
		writer.Visit(myVariantC);

		writeString = writer.Conclude();
	}

	{
		MyVariant myVariantA;
		MyVariant myVariantB;
		MyVariant myVariantC;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myVariantA);
		reader.Visit(myVariantB);
		reader.Visit(myVariantC);

		std::visit(core::Overload
			{
				[&](auto value) { FAIL("Variant with wrong type!"); },
				[&](bool value) { REQUIRE(value == s_Bool); },
			}, myVariantA);
		std::visit(core::Overload
			{
				[&](auto value) { FAIL("Variant with wrong type!"); },
				[&](int32 value) { REQUIRE(value == s_Int32); },
			}, myVariantB);
		std::visit(core::Overload
			{
				[&](auto value) { FAIL("Variant with wrong type!"); },
				[&](float value) { REQUIRE(value == s_Float); },
			}, myVariantC);
	}
}