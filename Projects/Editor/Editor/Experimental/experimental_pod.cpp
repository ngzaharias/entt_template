#include "experimental_pod.h"

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
		bool m_Bool = true;
		int m_Int = 666;
		float m_Float = 1337.f;
	};

	void InspectorBool(editor::Any data)
	{
		std::cout << "InspectorBool" << std::endl;
	}

	void InspectorInt(editor::Any data)
	{
		std::cout << "InspectorInt" << std::endl;
	}

	void InspectorFloat(editor::Any data)
	{
		std::cout << "InspectorFloat" << std::endl;
	}
}

void experimental_pod()
{
	const entt::hashed_string strInspector = "Inspector"_hs;

	entt::meta<bool>()
		.type("bool"_hs)
		.prop(core::strName, "bool")
		.func<&InspectorBool>(strInspector);

	entt::meta<int>()
		.type("int"_hs)
		.prop(core::strName, "int")
		.func<&InspectorInt>(strInspector);

	entt::meta<float>()
		.type("float"_hs)
		.prop(core::strName, "float")
		.func<&InspectorFloat>(strInspector);

	entt::meta<MyStruct>()
		.type("MyStruct"_hs)
		.prop(core::strName, "MyStruct")
		.data<&MyStruct::m_Float>("MyStruct::m_Float"_hs)
		.prop(core::strName, "m_Float")
		.data<&MyStruct::m_Int>("MyStruct::m_Int"_hs)
		.prop(core::strName, "m_Int")
		.data<&MyStruct::m_Bool>("MyStruct::m_Bool"_hs)
		.prop(core::strName, "m_Bool");

	entt::meta_any parentData = MyStruct();
	entt::meta_type parentType = parentData.type();
	for (const entt::meta_data& childMetaData : parentType.data())
	{
		entt::meta_any childData = childMetaData.get(parentData);
		if (entt::meta_func func = childMetaData.type().func(strInspector))
		{
			entt::meta_handle handle;
			editor::Any wrapper = { childData };
			func.invoke(handle, wrapper);
		}
	}
}