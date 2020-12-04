#include "experimental_variant.h"

#include <iostream>
#include <variant>
#include <entt/entt.hpp>
#include <entt/meta/meta.hpp>

namespace
{
	using TVariant = std::variant<bool, int>;
	struct MyStruct
	{
		TVariant m_Variant;
	};

	entt::meta_any getter(const MyStruct& value)
	{
		return value.m_Variant;
	};

	void setter(MyStruct& value, TVariant v)
	{
		value.m_Variant = v;
	};
}

void experimental_variant()
{
	entt::meta<MyStruct>()
		.data<&setter, &getter>("MyStruct::m_Variant"_hs);

	MyStruct myType;
	myType.m_Variant = true;

	entt::meta_any parentData = myType;
	entt::meta_type parentType = parentData.type();
	for (const entt::meta_data& childMetaData : parentType.data())
	{
		childMetaData.set(parentData, TVariant{ false });
		entt::meta_any childData = childMetaData.get(parentData);

		std::cout << "";
	}
}