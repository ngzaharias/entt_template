#include "EditorPCH.h"
#include "Editor/Application.h"

#include <Engine/AttributeHelpers.h>

#include <direct.h>
#include <entt/entt.hpp>
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/rapidjson.h>
#include <refl/refl.hpp>
#include <iostream>
#include <windows.h>

namespace json
{
	using Allocator = rapidjson::Document::AllocatorType;
	using Document = rapidjson::Document;
	using Path = str::String;
	using Pointer = rapidjson::Pointer;
	using StringBuffer = rapidjson::StringBuffer;
	using PrettyWriter = rapidjson::PrettyWriter<StringBuffer>;
	using Value = rapidjson::Value;
}

namespace
{

	struct Transaction
	{
		template<typename Type>
		Transaction(const char* path, const Type& value)
		{
			static_assert(false, "Unsupported Type!");
		}
		Transaction(const char* path, const bool& value)
		{
			json::Pointer("").Set(m_Document, value);
			m_Pointer = path;
		}
		Transaction(const char* path, const float& value)
		{
			json::Pointer("").Set(m_Document, value);
			m_Pointer = path;
		}
		Transaction(const char* path, const int32& value)
		{
			json::Pointer("").Set(m_Document, value);
			m_Pointer = path;
		}
		Transaction(const char* path, const Vector2f& value)
		{
			json::Value object;
			object.SetObject();
			object.AddMember("x", json::Value(value.x), m_Document.GetAllocator());
			object.AddMember("y", json::Value(value.y), m_Document.GetAllocator());
			json::Pointer("").Set(m_Document, object);
			m_Pointer = path;
		}

		void ApplyTo(json::Document& document) const
		{
			json::Pointer(m_Pointer.c_str()).Set(document, m_Document);
		}

	private:
		json::Document m_Document = { };
		json::Path m_Pointer = { };
	};

	void DocumentToType(const char* name, bool& value, const json::Value& parent)
	{
		value = parent.GetBool();
	}

	void DocumentToType(const char* name, float& value, const json::Value& parent)
	{
		value = parent.GetFloat();
	}

	void DocumentToType(const char* name, int32& value, const json::Value& parent)
	{
		value = parent.GetInt();
	}

	void DocumentToType(const char* name, Vector2f& value, const json::Value& parent)
	{
		const auto valueX = parent.FindMember("x");
		if (valueX != parent.MemberEnd())
			value.x = valueX->value.GetFloat();

		const auto valueY = parent.FindMember("y");
		if (valueY != parent.MemberEnd())
			value.y = valueY->value.GetFloat();
	}

	template<typename Type>
	void DocumentToType(const char* name, Type& value, const json::Value& parent)
	{
		constexpr bool isClass = std::is_class<Type>::value;
		constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
		if constexpr (isClass && isReflectable)
		{
			const auto end = parent.MemberEnd();
			for_each(refl::reflect<Type>().members, [&](auto field)
			{
				constexpr const char* name = reflect::GetFieldName(field);
				const auto child = parent.FindMember(name);
				if (child != end)
					DocumentToType(name, field(value), child->value);
			});
		}
	}

	template<typename Component>
	void DocumentToComponent(Component& value, const json::Document& document)
	{
		constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
		const char* name = get_display_name(descriptor);

		const auto child = document.FindMember(name);
		if (child != document.MemberEnd())
			DocumentToType(name, value, child->value);
	}

	void TypeToDocument(const char* name, const bool& value, json::Value& parent, json::Allocator& allocator)
	{
		parent.SetBool(value);
	}

	void TypeToDocument(const char* name, const float& value, json::Value& parent, json::Allocator& allocator)
	{
		parent.SetFloat(value);
	}

	void TypeToDocument(const char* name, const int32& value, json::Value& parent, json::Allocator& allocator)
	{
		parent.SetInt(value);
	}

	template<typename Type>
	void TypeToDocument(const char* name, const Type& value, json::Value& parent, json::Allocator& allocator)
	{
		constexpr bool isClass = std::is_class<Type>::value;
		constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
		if constexpr (isClass && isReflectable)
		{
			for_each(refl::reflect<Type>().members, [&](auto field)
			{
				constexpr const char* name = reflect::GetFieldName(field);

				json::Value child; child.SetObject();
				TypeToDocument(name, field(value), child, allocator);
				parent.AddMember(rapidjson::StringRef(name), child, allocator);
			});
		}
	}

	template<typename Component>
	void ComponentToDocument(const Component& value, json::Document& parent)
	{
		constexpr refl::type_descriptor descriptor = refl::reflect<Component>();
		const char* name = get_display_name(descriptor);

		parent.SetObject();
		json::Value child; child.SetObject();
		TypeToDocument(name, value, child, parent.GetAllocator());
		parent.AddMember(rapidjson::StringRef(name), child, parent.GetAllocator());
	}
}

struct AsBools { bool m_BoolA = false; bool m_BoolB = false; };
struct AsFloat { float m_FloatA = 0.f; };
struct AsInts { int m_IntA = 0; int m_IntB = 0; int m_IntC = 0; };
struct AsStruct { AsBools m_Bools; AsFloat m_Floats; AsInts m_Ints; };

REFL_AUTO(type(AsBools), field(m_BoolA), field(m_BoolB))
REFL_AUTO(type(AsFloat), field(m_FloatA))
REFL_AUTO(type(AsInts), field(m_IntA), field(m_IntB), field(m_IntC))
REFL_AUTO(type(AsStruct), field(m_Bools), field(m_Floats), field(m_Ints))

int main(int agrc, char* argv[])
{
	{
		using Transactions = std::vector<Transaction>;
		json::Document document;

		// save to document
		{
			AsStruct asStruct;
			asStruct.m_Bools.m_BoolA = true;
			asStruct.m_Floats.m_FloatA = 0.12345f;
			asStruct.m_Ints.m_IntA = 1337;

			ComponentToDocument(asStruct, document);

			json::StringBuffer buffer;
			json::PrettyWriter writer(buffer);
			document.Accept(writer);

			std::cout << "Saving\n";
			std::cout << buffer.GetString() << '\n';
		}

		// load from document
		{
			AsStruct asStruct;
			DocumentToComponent(asStruct, document);

			std::cout << "Loading\n";
			std::cout << asStruct.m_Bools.m_BoolA << '\n';
			std::cout << asStruct.m_Bools.m_BoolB << '\n';
			std::cout << asStruct.m_Floats.m_FloatA << '\n';
			std::cout << asStruct.m_Ints.m_IntA << '\n';
			std::cout << asStruct.m_Ints.m_IntB << '\n';
			std::cout << asStruct.m_Ints.m_IntC << '\n';
		}

		// transactions
		{
			Transactions transactions;
			transactions.emplace_back("/AsStruct/m_Ints/m_IntA", 666);

			for (const Transaction& transaction : transactions)
				transaction.ApplyTo(document);

			AsStruct asStruct;
			DocumentToComponent(asStruct, document);

			std::cout << "Transactions\n";
			std::cout << asStruct.m_Bools.m_BoolA << '\n';
			std::cout << asStruct.m_Bools.m_BoolB << '\n';
			std::cout << asStruct.m_Floats.m_FloatA << '\n';
			std::cout << asStruct.m_Ints.m_IntA << '\n';
			std::cout << asStruct.m_Ints.m_IntB << '\n';
			std::cout << asStruct.m_Ints.m_IntC << '\n';
		}
	}

	// #todo: we shouldn't need to change directory
	// #fixme: this won't work when running the game via the exe (only the solution)
	_chdir("../../");

	editor::Application* application = new editor::Application();
	application->Execute(agrc, argv);
	delete(application);

	return 0;
}