#pragma once

#include <Engine/PhysicsMaterialAsset.h>
#include <Engine/SoundAsset.h>
#include <Engine/TextureAsset.h>

#include <map>
#include <variant>
#include <vector>
#include <refl/refl.hpp>
#include <SFML/System/Vector3.hpp>

namespace example
{
	struct AsBools { bool m_BoolA; bool m_BoolB; };
	struct AsInts { int m_IntA; int m_IntB; int m_IntC; };
	struct AsFloat { float m_FloatA; };
	struct AsStruct { AsBools m_Bools; };

	struct NoReflect { bool m_Bool; };
	struct YesReflect
	{
		bool m_Bool = false;
		int m_Int = 0;
		float m_Float = 0.f;
		AsFloat m_Struct = AsFloat();

		bool operator==(const YesReflect& rhs) const
		{
			return m_Bool == rhs.m_Bool
				&& m_Int == rhs.m_Int
				&& m_Float == rhs.m_Float;
		}

		bool operator!=(const YesReflect& rhs) const
		{
			return m_Bool != rhs.m_Bool
				|| m_Int != rhs.m_Int
				|| m_Float != rhs.m_Float;
		}

		bool operator<(const YesReflect& rhs) const
		{
			return m_Bool < rhs.m_Bool
				&& m_Int < rhs.m_Int
				&& m_Float < rhs.m_Float;
		}
	};

	struct Component
	{
		// trivials
		bool m_Bool = true;
		int m_Int = 1337;
		float m_Float = 0.666f;

		// struct
		YesReflect m_YesReflect = YesReflect();
		NoReflect m_NoReflect = NoReflect();

		// customised
		sf::Vector3f m_Vector3 = { 1.f, 2.f, 3.f };

		// Assets
		// #todo: should these be handles or should it be the guid?
		physics::MaterialHandle m_PhysicsMaterial;
		audio::SoundHandle m_Sound;
		render::TextureHandle m_Texture;

		// maps
		std::map<int, int> m_MapA = { {1, 10}, {2, 10}, {3, 10} };
		std::map<int, YesReflect> m_MapB = { { 1, YesReflect()}, { 2, YesReflect()} };
		std::map<YesReflect, int> m_MapC = { { YesReflect(), 10} };
		std::map<YesReflect, YesReflect> m_MapD = { {YesReflect(), YesReflect()} };

		// vectors
		std::vector<int> m_VectorA = { 1, 2, 3, 4, 5 };
		std::vector<YesReflect> m_VectorB = { YesReflect(), YesReflect(), YesReflect() };

		// variant
		std::variant<bool, int, float, AsBools, AsInts, AsFloat, AsStruct> m_Variant = AsInts();
	};
}

REFL_AUTO(type(example::AsBools), field(m_BoolA), field(m_BoolB))
REFL_AUTO(type(example::AsInts), field(m_IntA), field(m_IntB), field(m_IntC))
REFL_AUTO(type(example::AsFloat), field(m_FloatA))
REFL_AUTO(type(example::AsStruct), field(m_Bools))

REFL_AUTO
(
	type(example::YesReflect)
	, field(m_Bool)
	, field(m_Int)
	, field(m_Float)
	, field(m_Struct)
)

REFL_AUTO
(
	type(example::Component)
	, field(m_Bool)
	, field(m_Int)
	, field(m_Float, prop::Range(0, 100))
	, field(m_YesReflect)
	, field(m_NoReflect)
	, field(m_PhysicsMaterial)
	, field(m_Sound)
	, field(m_Texture)
	, field(m_Vector3)
	, field(m_MapA)
	, field(m_MapB)
	, field(m_MapC)
	, field(m_MapD)
	, field(m_VectorA)
	, field(m_VectorB)
	, field(m_Variant)
)