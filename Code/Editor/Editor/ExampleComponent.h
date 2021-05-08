#pragma once

#include <Engine/AttributeTypes.h>
#include <Engine/PhysicsMaterialAsset.h>
#include <Engine/SoundAsset.h>
#include <Engine/TextureAsset.h>

namespace example
{
	struct AsBools { bool m_BoolA; };
	struct AsFloat { float m_FloatA; float m_FloatB; };
	struct AsInts { int m_IntA; int m_IntB; int m_IntC; };
	struct AsStruct { AsBools m_Bools; };

	struct YesReflect
	{
		bool m_Bool = false;
		float m_Float = 0.f;
		int32 m_Int32 = 0;
		AsFloat m_Struct = AsFloat();

		bool operator==(const YesReflect& rhs) const
		{
			return m_Bool == rhs.m_Bool
				&& m_Float == rhs.m_Float
				&& m_Int32 == rhs.m_Int32;
		}

		bool operator!=(const YesReflect& rhs) const
		{
			return m_Bool != rhs.m_Bool
				|| m_Float != rhs.m_Float
				|| m_Int32 != rhs.m_Int32;
		}

		bool operator<(const YesReflect& rhs) const
		{
			return m_Bool < rhs.m_Bool
				&& m_Float < rhs.m_Float
				&& m_Int32 < rhs.m_Int32;
		}
	};

	struct NoReflect { bool m_Bool; };

	struct ExampleComponent
	{
		// trivial
		bool m_Bool = true;
		float m_Float = 0.666f;
		int32 m_Int32 = -1337;
		uint32 m_Uint32 = 4444;
		sf::Vector2f m_Vector2f = { 1.f, 2.f };
		sf::Vector2i m_Vector2i = { 1, 2 };
		sf::Vector2u m_Vector2u = { 1, 2 };
		sf::Vector3f m_Vector3f = { 1.f, 2.f, 3.f };
		sf::Vector3i m_Vector3i = { 1, 2, 3 };

		// struct
		YesReflect m_YesReflect = YesReflect();
		NoReflect m_NoReflect = NoReflect();

		// asset
		physics::MaterialHandle m_PhysicsMaterial;
		audio::SoundHandle m_Sound;
		render::TextureHandle m_Texture;

		// map
		Map<int32, int32> m_MapA = { {1, 10}, {2, 10}, {3, 10} };
		Map<int32, YesReflect> m_MapB = { { 1, YesReflect()}, { 2, YesReflect()} };
		Map<YesReflect, int32> m_MapC = { { YesReflect(), 10} };
		Map<YesReflect, YesReflect> m_MapD = { {YesReflect(), YesReflect()} };

		// set
		Set<int32> m_SetA = { 1, 2, 3 };

		// vector
		Array<int32> m_VectorA = { 1, 2, 3 };
		Array<AsInts> m_VectorB = { AsInts(), AsInts(), AsInts() };

		// variant
		Variant<bool, float, int32, AsBools, AsFloat, AsInts, AsStruct> m_Variant = AsInts();
	};
}

REFL_AUTO(type(example::AsBools), field(m_BoolA))
REFL_AUTO(type(example::AsFloat), field(m_FloatA), field(m_FloatB))
REFL_AUTO(type(example::AsInts), field(m_IntA), field(m_IntB), field(m_IntC))
REFL_AUTO(type(example::AsStruct), field(m_Bools))

REFL_AUTO
(
	type(example::YesReflect)
	, field(m_Bool)
	, field(m_Float)
	, field(m_Int32)
	, field(m_Struct)
)

REFL_AUTO
(
	type(example::ExampleComponent)
	, field(m_Bool)
	, field(m_Float, attr::Range(0.f, 100.f))
	, field(m_Int32, attr::Range(0.f, 100.f))
	, field(m_Uint32)
	, field(m_Vector2f, attr::Range(0.f, 100.f))
	, field(m_Vector2i, attr::Range(0.f, 100.f))
	, field(m_Vector2u, attr::Range(0.f, 100.f))
	, field(m_Vector3f, attr::Range(0.f, 100.f))
	, field(m_Vector3i, attr::Range(0.f, 100.f))
	, field(m_YesReflect)
	, field(m_PhysicsMaterial)
	, field(m_Sound)
	, field(m_Texture)
	, field(m_MapA)
	, field(m_MapB)
	, field(m_MapC)
	, field(m_MapD)
	, field(m_SetA)
	, field(m_VectorA)
	, field(m_VectorB)
	, field(m_Variant)
)

//refl::descriptor::type_descriptor reflected = refl::reflect<MyStruct>();
//refl::descriptor::member_list<MyStruct> members = reflected.members;
//for_each(refl::reflect<MyStruct>().members, [&](auto member)
//{
//	refl::descriptor::field_descriptor<MyStruct, 0>& field = member;
//});