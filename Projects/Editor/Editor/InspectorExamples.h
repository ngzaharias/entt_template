#pragma once

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

	struct Struct
	{
		bool m_Bool = false;
		int m_Int = 0;
		float m_Float = 0.f;

		bool operator<(const Struct& rhs) const
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
		Struct m_Struct = Struct();

		// customised
		sf::Vector3f m_Custom = { 1.f, 2.f, 3.f };

		// maps
		std::map<int, int> m_MapA = { {1, 10} };
		std::map<int, Struct> m_MapB = { {1, Struct()} };
		std::map<Struct, int> m_MapC = { {Struct(), 10} };
		std::map<Struct, Struct> m_MapD = { {Struct(), Struct()} };

		// vectors
		std::vector<int> m_VectorA = { 1, 2, 3, 4, 5 };
		std::vector<Struct> m_VectorB = { Struct(), Struct(), Struct() };

		// variant
		std::variant<bool, int, float, AsBools, AsInts, AsFloat> m_Variant = AsInts();
	};
}

REFL_AUTO(type(example::AsBools), field(m_BoolA), field(m_BoolB))
REFL_AUTO(type(example::AsInts), field(m_IntA), field(m_IntB), field(m_IntC))
REFL_AUTO(type(example::AsFloat), field(m_FloatA))

REFL_AUTO
(
	type(example::Struct)
	, field(m_Bool)
	, field(m_Int)
	, field(m_Float)
)

REFL_AUTO
(
	type(example::Component)
	, field(m_Bool)
	, field(m_Int)
	, field(m_Float)
	, field(m_Struct)
	, field(m_Custom)
	, field(m_MapA)
	, field(m_MapB)
	, field(m_MapC)
	, field(m_MapD)
	, field(m_VectorA)
	, field(m_VectorB)
	, field(m_Variant)
)