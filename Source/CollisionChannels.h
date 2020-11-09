#pragma once

namespace physics
{
	enum class EChannel
	{
		RigidDynamic
		, RigidStatic

		, EngineChannel1
		, EngineChannel2
		, EngineChannel3
		, EngineChannel4
		, EngineChannel5
		, EngineChannel6

		, GameChannel1
		, GameChannel2
		, GameChannel3
		, GameChannel4
		, GameChannel5
		, GameChannel6
		, GameChannel7
		, GameChannel8
		, GameChannel9
		, GameChannel10
		, GameChannel11
		, GameChannel12
		, GameChannel13
		, GameChannel14
		, GameChannel15
		, GameChannel16
		, GameChannel17
		, GameChannel18

		, _Unused1
		, _Unused2
		, _Unused3
		, _Unused4
		, _Unused5
		, _Unused6

		, _MAX
	};

	enum class EResponse
	{
		Ignore
		, Overlap
		, Block
	};
}