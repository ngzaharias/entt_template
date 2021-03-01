#pragma once

#include <Engine/AttributeHelpers.h>
#include <Engine/VariantHelpers.h>

#include <map>
#include <set>
#include <variant>
#include <vector>
#include <rapidjson/allocators.h>
#include <rapidjson/pointer.h>
#include <refl/refl.hpp>

namespace json
{
	using Allocator = rapidjson::Document::AllocatorType;
	using Document = rapidjson::Document;
	using Object = rapidjson::Value;
	using Pointer = rapidjson::Pointer;
}

namespace
{
	void CopyToType(const json::Object& object, bool& value)
	{
		if (object.IsNull())
			return;

		value = object.GetBool();
	}

	void CopyToType(const json::Object& object, float& value)
	{
		if (object.IsNull())
			return;

		value = object.GetFloat();
	}

	void CopyToType(const json::Object& object, int32& value)
	{
		if (object.IsNull())
			return;

		value = object.GetInt();
	}

	void CopyToType(const json::Object& object, uint32& value)
	{
		if (object.IsNull())
			return;

		value = object.GetUint();
	}

	void CopyToType(const json::Object& object, Vector2f& value)
	{
		if (object.IsNull())
			return;

		const auto valueX = object.FindMember("x");
		if (valueX != object.MemberEnd())
			value.x = valueX->value.GetFloat();
		const auto valueY = object.FindMember("y");
		if (valueY != object.MemberEnd())
			value.y = valueY->value.GetFloat();
	}
	
	void CopyToType(const json::Object& object, Vector2i& value)
	{
		if (object.IsNull())
			return;

		const auto valueX = object.FindMember("x");
		if (valueX != object.MemberEnd())
			value.x = valueX->value.GetInt();
		const auto valueY = object.FindMember("y");
		if (valueY != object.MemberEnd())
			value.y = valueY->value.GetInt();
	}
	
	void CopyToType(const json::Object& object, Vector2u& value)
	{
		if (object.IsNull())
			return;

		const auto valueX = object.FindMember("x");
		if (valueX != object.MemberEnd())
			value.x = valueX->value.GetUint();
		const auto valueY = object.FindMember("y");
		if (valueY != object.MemberEnd())
			value.y = valueY->value.GetUint();
	}

	void CopyToType(const json::Object& object, Vector3f& value)
	{
		if (object.IsNull())
			return;

		const auto valueX = object.FindMember("x");
		if (valueX != object.MemberEnd())
			value.x = valueX->value.GetFloat();
		const auto valueY = object.FindMember("y");
		if (valueY != object.MemberEnd())
			value.y = valueY->value.GetFloat(); 
		const auto valueZ = object.FindMember("z");
		if (valueZ != object.MemberEnd())
			value.z = valueY->value.GetFloat();
	}

	void CopyToType(const json::Object& object, Vector3i& value)
	{
		if (object.IsNull())
			return;

		const auto valueX = object.FindMember("x");
		if (valueX != object.MemberEnd())
			value.x = valueX->value.GetInt();
		const auto valueY = object.FindMember("y");
		if (valueY != object.MemberEnd())
			value.y = valueY->value.GetInt();
		const auto valueZ = object.FindMember("z");
		if (valueZ != object.MemberEnd())
			value.z = valueY->value.GetInt();
	}

	void CopyToType(const json::Object& object, str::String& value)
	{
		if (object.IsNull())
			return;

		value = object.GetString();
	}

	template<typename Container>
	void CopyToContainer(const json::Object& object, Container& value)
	{
		if (object.IsNull())
			return;
	}

	template<typename Key, typename Val>
	void CopyToContainer(const json::Object& object, std::map<Key, Val>& value)
	{
		if (object.IsNull())
			return;

		const auto& members = object.GetArray();
		const size_t count = static_cast<size_t>(members.Size());

		value.clear();
		for (const json::Object& itrObject : members)
		{
			Key key = { };
			const auto keyObject = itrObject.FindMember("Key");
			if (keyObject != itrObject.MemberEnd())
				CopyToType(keyObject->value, key);

			Val val = { };
			const auto valObject = itrObject.FindMember("Value");
			if (valObject != itrObject.MemberEnd())
				CopyToType(valObject->value, val);

			value[key] = val;
		}
	}

	template<typename Type>
	void CopyToContainer(const json::Object& object, std::vector<Type>& value)
	{
		if (object.IsNull())
			return;

		const auto& members = object.GetArray();
		const size_t count = static_cast<size_t>(members.Size());

		value.resize(count);
		for (int32 i = 0; i < count; ++i)
			CopyToType(members[i], value[i]);
	}

	template <typename Type>
	void CopyToType(const json::Object& object, Type& value)
	{
		if (object.IsNull())
			return;

		constexpr bool isClass = std::is_class<Type>::value;
		constexpr bool isContainer = refl::trait::is_container<Type>::value;
		constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
		constexpr bool isVariant = core::IsVariant<Type>::value;

		if constexpr (isClass && isReflectable)
		{
			for_each(refl::reflect<Type>().members, [&](auto field)
			{
				constexpr const char* name = reflect::GetFieldName(field);
				const auto member = object.FindMember(name);
				if (member != object.MemberEnd())
					CopyToType(member->value, field(value));
			});
		}
		else if constexpr (isVariant)
		{
			std::visit([&](auto& subValue)
			{
			}, value);
		}
		else if constexpr (isContainer)
		{
			CopyToContainer(object, value);
		}
		else
		{
			// #todo: enable
			//static_assert(false, ":::CopyToType Unhandled Type!");
		}
	}

	template <typename Component>
	void CopyToEntity(const editor::Record& record, entt::registry& registry)
	{
		if (Component* component = registry.try_get<Component>(record.m_Entity))
		{
			constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
			const char* name = get_display_name(descriptor);
			const auto member = record.m_Document.FindMember(name);
			if (member != record.m_Document.MemberEnd())
				CopyToType(member->value, *component);
		}
	}
}

template <typename ...Types>
void editor::Historian::CopyToEntity(const editor::Record& record, entt::registry& registry, core::TypeList<Types...> types)
{
	(::CopyToEntity<Types>(record, registry), ...);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

namespace
{
	void CopyToDocument(const bool& value, json::Object& object, json::Allocator& allocator)
	{
		object.SetBool(value);
	}

	void CopyToDocument(const float& value, json::Object& object, json::Allocator& allocator)
	{
		object.SetFloat(value);
	}

	void CopyToDocument(const int32& value, json::Object& object, json::Allocator& allocator)
	{
		object.SetInt(value);
	}

	void CopyToDocument(const Vector2f& value, json::Object& object, json::Allocator& allocator)
	{
		object.SetObject();
		object.AddMember("x", json::Object(value.x), allocator);
		object.AddMember("y", json::Object(value.y), allocator);
	}

	void CopyToDocument(const Vector2i& value, json::Object& object, json::Allocator& allocator)
	{
		object.SetObject();
		object.AddMember("x", json::Object(value.x), allocator);
		object.AddMember("y", json::Object(value.y), allocator);
	}

	void CopyToDocument(const Vector2u& value, json::Object& object, json::Allocator& allocator)
	{
		object.SetObject();
		object.AddMember("x", json::Object(value.x), allocator);
		object.AddMember("y", json::Object(value.y), allocator);
	}

	void CopyToDocument(const Vector3f& value, json::Object& object, json::Allocator& allocator)
	{
		object.SetObject();
		object.AddMember("x", json::Object(value.x), allocator);
		object.AddMember("y", json::Object(value.y), allocator);
		object.AddMember("z", json::Object(value.z), allocator);
	}

	void CopyToDocument(const Vector3i& value, json::Object& object, json::Allocator& allocator)
	{
		object.SetObject();
		object.AddMember("x", json::Object(value.x), allocator);
		object.AddMember("y", json::Object(value.y), allocator);
		object.AddMember("z", json::Object(value.z), allocator);
	}

	void CopyToDocument(const str::String& value, json::Object& object, json::Allocator& allocator)
	{
		const rapidjson::SizeType length = static_cast<rapidjson::SizeType>(value.length());
		object.SetString(value.c_str(), length);
	}

	template<typename Container>
	void CopyContainerToDocument(const Container& value, json::Object& object, json::Allocator& allocator)
	{
	}

	template<typename Key, typename Val>
	void CopyContainerToDocument(const std::map<Key, Val>& value, json::Object& object, json::Allocator& allocator)
	{
		using Iterator = typename std::map<Key, Val>::const_iterator;
		
		object.SetArray();

		Iterator itr = value.cbegin();
		Iterator end = value.cend();
		for (int32 i = 0; itr != end; ++itr, ++i)
		{
			json::Object itrObject; itrObject.SetObject();
			json::Object keyObject; keyObject.SetObject();
			json::Object valObject; valObject.SetObject();
			CopyToDocument(itr->first, keyObject, allocator);
			CopyToDocument(itr->second, valObject, allocator);
			itrObject.AddMember("Key", keyObject, allocator);
			itrObject.AddMember("Value", valObject, allocator);
			object.PushBack(itrObject, allocator);
		}
	}

	template<typename Type>
	void CopyContainerToDocument(const std::vector<Type>& value, json::Object& object, json::Allocator& allocator)
	{
		using Iterator = typename std::vector<Type>::const_iterator;

		object.SetArray();

		Iterator itr = value.cbegin();
		Iterator end = value.cend();
		for (int32 i = 0; itr != end; ++itr, ++i)
		{
			json::Object childObject; childObject.SetObject();
			CopyToDocument(*itr, childObject, allocator);
			object.PushBack(childObject, allocator);
		}
	}

	template <typename Type>
	void CopyToDocument(const Type& value, json::Object& object, json::Allocator& allocator)
	{
		constexpr bool isClass = std::is_class<Type>::value;
		constexpr bool isContainer = refl::trait::is_container<Type>::value;
		constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
		constexpr bool isVariant = core::IsVariant<Type>::value;

		if constexpr (isClass && isReflectable)
		{
			for_each(refl::reflect<Type>().members, [&](auto field)
			{
				constexpr const char* name = reflect::GetFieldName(field);

				json::Object childObject; childObject.SetObject();
				CopyToDocument(field(value), childObject, allocator);
				object.AddMember(rapidjson::StringRef(name), childObject, allocator);
			});
		}
		else if constexpr (isVariant)
		{
			std::visit([&](auto& subValue)
			{
			}, value);
		}
		else if constexpr (isContainer)
		{
			CopyContainerToDocument(value, object, allocator);
		}
		else
		{
			// #todo: enable
			//static_assert(false, ":::CopyToDocument Unhandled Type!");
		}
	}

	template <typename Component>
	void CopyToRecord(const entt::registry& registry, editor::Record& record)
	{
		if (const Component* component = registry.try_get<Component>(record.m_Entity))
		{
			constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
			const char* name = get_display_name(descriptor);

			json::Object childObject; childObject.SetObject();
			CopyToDocument(*component, childObject, record.m_Document.GetAllocator());
			record.m_Document.AddMember(rapidjson::StringRef(name), childObject, record.m_Document.GetAllocator());
		}
	}
}

template <typename ...Types>
void editor::Historian::CopyToRecord(const entt::registry& registry, editor::Record& record, core::TypeList<Types...> types)
{
	record.m_Document.SetObject();
	(::CopyToRecord<Types>(registry, record), ...);
}