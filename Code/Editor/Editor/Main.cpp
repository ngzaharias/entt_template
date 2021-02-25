#include "EditorPCH.h"
#include "Editor/Application.h"

#include <Engine/AttributeHelpers.h>

#include <direct.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <refl/refl.hpp>
#include <iostream>
#include <windows.h>

struct AsBools { bool m_BoolA = false; bool m_BoolB = false; };
struct AsInts { int m_IntA = 0; int m_IntB = 0; int m_IntC = 0; };
struct AsFloat { float m_FloatA = 0.f; };
struct AsStruct { AsBools m_Bools; AsInts m_Ints; AsFloat m_Floats; };

REFL_AUTO(type(AsBools), field(m_BoolA), field(m_BoolB))
REFL_AUTO(type(AsInts), field(m_IntA), field(m_IntB), field(m_IntC))
REFL_AUTO(type(AsFloat), field(m_FloatA))
REFL_AUTO(type(AsStruct), field(m_Bools), field(m_Ints), field(m_Floats))

static std::vector<str::String> s_Transactions = { };

void Inspect(const bool& parent, rapidjson::Value* value, str::String transactionId)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (value)
	{
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << transactionId.c_str() << ": [" << value->GetBool() << "]\n";
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << transactionId.c_str() << ": [" << parent << "]\n";
	}
}

void Inspect(const float& parent, rapidjson::Value* value, str::String transactionId)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (value)
	{
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << transactionId.c_str() << ": [" << value->GetFloat() << "]\n";
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << transactionId.c_str() << ": [" << parent << "]\n";
	}
}

void Inspect(const int32& parent, rapidjson::Value* value, str::String transactionId)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (value)
	{
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << transactionId.c_str() << ": [" << value->GetInt() << "]\n";
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << transactionId.c_str() << ": [" << parent << "]\n";
	}
}

template<typename Type>
void Inspect(const Type& parent, rapidjson::Value* value, str::String transactionId)
{
	constexpr bool isClass = std::is_class<Type>::value;
	constexpr bool isReflectable = refl::trait::is_reflectable<Type>::value;
	if constexpr (isClass && isReflectable)
	{
		for_each(refl::reflect<Type>().members, [&](auto field)
		{
			constexpr const char* name = reflect::GetFieldName(field);
			str::String childId = transactionId + '.' + name;
			rapidjson::Value* childValue = nullptr;

			if (value)
			{
				auto& member = value->FindMember(name);
				if (member != value->MemberEnd())
					childValue = &member->value;
			}

			Inspect(field(parent), childValue, childId);
		});
	}
	else
	{
	}
}

int main(int agrc, char* argv[])
{
	{
		const char* file = "{ \"m_Data\": { \"m_Bools\": { \"m_BoolB\": true }, \"m_Ints\": { \"m_IntC\": 1337 }, \"m_Floats\": { \"m_FloatA\": 0.12345 } } }";

		AsStruct parentOrdefault;
		rapidjson::Document document;
		document.Parse(file);

		auto& member = document.FindMember("m_Data");
		if (member != document.MemberEnd())
			Inspect(parentOrdefault, &member->value, "m_Data");
	}

	// #todo: we shouldn't need to change directory
	// #fixme: this won't work when running the game via the exe (only the solution)
	_chdir("../../");

	editor::Application* application = new editor::Application();
	application->Execute(agrc, argv);
	delete(application);

	return 0;
}