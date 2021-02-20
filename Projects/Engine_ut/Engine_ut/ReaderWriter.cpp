#include <Catch2/catch.hpp>

#include <Engine/String.h>
#include <Engine/StringView.h>
#include <Engine/Types.h>
#include <Engine/Vector2.h>
#include <Engine/Vector3.h>

#include <Engine/Reader.h>
#include <Engine/Writer.h>

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

		writeString = writer.Conclude();
	}

	{
		bool myBool; int32 myInt; uint32 myUint;
		double myDouble; float myFloat;

		serialize::Reader reader(writeString.c_str());
		reader.Visit(myBool);
		reader.Visit(myInt);
		reader.Visit(myUint);
		reader.Visit(myDouble);
		reader.Visit(myFloat);

		REQUIRE(myBool		== true);
		REQUIRE(myInt		== -1337);
		REQUIRE(myUint		== 666);
		REQUIRE(myDouble	== 1.111111111);
		REQUIRE(myFloat		== 1.23456789f);
	}
}
