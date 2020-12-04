#include "experimental_containers.h"

#include "Editor/PropertyDefines.h"

#include <Engine/StringTable.h>

#include <iostream>
#include <variant>
#include <entt/entt.hpp>
#include <entt/meta/meta.hpp>

namespace
{
	struct MyStruct
	{
		int m_MyValue;

		bool operator==(const MyStruct& rhs) const { return this->m_MyValue == rhs.m_MyValue; }
	};
	const MyStruct s_Sample = MyStruct();

	struct ComplexContainers
	{
		MyStruct m_Array[3] = { s_Sample, s_Sample, s_Sample };
		std::map<int, MyStruct> m_Map = { {1,s_Sample}, {2,s_Sample}, {3,s_Sample} };
		std::vector<MyStruct> m_Vector = { s_Sample, s_Sample, s_Sample };
	};

	struct TrivialContainers
	{
		int m_Array[3] = { 1, 2, 3 };
		std::map<int, int> m_Map = { {1,1}, {2,2}, {3,3} };
		std::vector<int> m_Vector = { 1, 2, 3 };
	};



	void InspectorInt(editor::Any data)
	{
		std::cout << "InspectorInt" << std::endl;
	}
}

void experimental_containers()
{
	const entt::hashed_string strInspector = "Inspector"_hs;

	entt::meta<int>()
		.type("int"_hs)
		.prop(core::strName, "int")
		.func<&InspectorInt>(strInspector);

	// TrivialContainers
	{
		entt::meta<TrivialContainers>()
			.type("TrivialContainers"_hs)
			.prop(core::strName, "TrivialContainers")
			.data<&TrivialContainers::m_Array>("TrivialContainers::m_Array"_hs)
			.prop(core::strName, "m_Array")
			.data<&TrivialContainers::m_Map>("TrivialContainers::m_Map"_hs)
			.prop(core::strName, "m_Map")
			.data<&TrivialContainers::m_Vector>("TrivialContainers::m_Vector"_hs)
			.prop(core::strName, "m_Vector");

		entt::meta_any parentData = TrivialContainers();
		entt::meta_type parentType = parentData.type();
		for (const entt::meta_data& childMetaData : parentType.data())
		{
			entt::meta_any childData = childMetaData.get(parentData);
		}
	}

	// ComplexContainers
	{
		entt::meta<ComplexContainers>()
			.type("ComplexContainers"_hs)
			.prop(core::strName, "ComplexContainers")
			.data<&ComplexContainers::m_Array>("ComplexContainers::m_Array"_hs)
			.prop(core::strName, "m_Array")
			.data<&ComplexContainers::m_Map>("ComplexContainers::m_Map"_hs)
			.prop(core::strName, "m_Map")
			.data<&ComplexContainers::m_Vector>("ComplexContainers::m_Vector"_hs)
			.prop(core::strName, "m_Vector");

		entt::meta_any parentData = ComplexContainers();
		entt::meta_type parentType = parentData.type();
		for (const entt::meta_data& childMetaData : parentType.data())
		{
			entt::meta_any childData = childMetaData.get(parentData);
		}
	}
}