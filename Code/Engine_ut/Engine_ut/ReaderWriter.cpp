#include <Catch2/catch.hpp>

#include <Engine/Name.h>
#include <Engine/String.h>
#include <Engine/StringView.h>
#include <Engine/Types.h>
#include <Engine/Vector2.h>
#include <Engine/Vector3.h>

#include <Engine/Reader.h>
#include <Engine/Writer.h>

#include <refl/refl.hpp>

TEST_CASE("Trivials")
{
	str::String writeString;

	{
		serialize::Writer writer;
		writer.Visit(true);
		writer.Visit(-1337);
		writer.Visit(666);
		writer.Visit(1.111111111);
		writer.Visit(1.23456789f);
		writer.Visit(NAME("Nicholas"));
		writer.Visit(str::String("HelloWorld"));
		writer.Visit(Vector2f(1.f, 2.f));
		writer.Visit(Vector2i(-1, -2));
		writer.Visit(Vector2u(1, 2));
		writer.Visit(Vector3f(1.f, 2.f, 3.f));
		writer.Visit(Vector3i(-1, -2, -3));

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

		REQUIRE(myBool		== true);
		REQUIRE(myInt		== -1337);
		REQUIRE(myUint		== 666);
		REQUIRE(myDouble	== 1.111111111);
		REQUIRE(myFloat		== 1.23456789f);
		REQUIRE(myName		== NAME("Nicholas"));
		REQUIRE(myString	== "HelloWorld");
		REQUIRE(myVector2f  == Vector2f(1.f, 2.f));
		REQUIRE(myVector2i  == Vector2i(-1, -2));
		REQUIRE(myVector2u  == Vector2u(1, 2));
		REQUIRE(myVector3f  == Vector3f(1.f, 2.f, 3.f));
		REQUIRE(myVector3i  == Vector3i(-1, -2, -3));
	}
}

struct MyStruct
{
	bool myBool;
	int32 myInt;
	uint32 myUint;
	double myDouble;
	float myFloat;
};

REFL_AUTO
(
	type(MyStruct)
	, field(myBool)
	, field(myInt)
	, field(myUint)
	, field(myDouble)
	, field(myFloat)
)

TEST_CASE("Struct")
{
	str::String writeString;

	{
		MyStruct myStruct = { true, -1337, 666, 1.111111111, 1.23456789f };

		serialize::Writer writer;
		writer.Visit(myStruct);

		writeString = writer.Conclude();
	}

	{
		MyStruct myStruct;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myStruct);

		REQUIRE(myStruct.myBool   == true);
		REQUIRE(myStruct.myInt    == -1337);
		REQUIRE(myStruct.myUint   == 666);
		REQUIRE(myStruct.myDouble == 1.111111111);
		REQUIRE(myStruct.myFloat  == 1.23456789f);
	}
}

TEST_CASE("Vector")
{
	str::String writeString;

	{
		std::vector<int32> myVector = { 1, 2, 3, 4, 5 };

		serialize::Writer writer;
		writer.Visit(myVector);

		writeString = writer.Conclude();
	}

	{
		std::vector<int32> myVector;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myVector);

		REQUIRE(myVector.size() == 5);
		for (int32 i = 0; i < myVector.size(); ++i)
			REQUIRE(myVector[i] == i + 1);
	}
}