#include "EditorPCH.h"
#include "Editor/Transaction.h"

#include <rapidjson/pointer.h>

editor::Transaction::Transaction(const char* address, const bool& value)
	: m_Address(address)
{
	rapidjson::Pointer("").Set(m_Document, value);
}

editor::Transaction::Transaction(const char* address, const float& value)
	: m_Address(address)
{
	rapidjson::Pointer("").Set(m_Document, value);
}

editor::Transaction::Transaction(const char* address, const int32& value)
	: m_Address(address)
{
	rapidjson::Pointer("").Set(m_Document, value);
}

editor::Transaction::Transaction(const char* address, const uint32& value)
	: m_Address(address)
{
	rapidjson::Pointer("").Set(m_Document, value);
}

editor::Transaction::Transaction(const char* address, const Vector2f& value)
	: m_Address(address)
{
	json::Object object;
	object.SetObject();
	object.AddMember("x", json::Object(value.x), m_Document.GetAllocator());
	object.AddMember("y", json::Object(value.y), m_Document.GetAllocator());
	rapidjson::Pointer("").Set(m_Document, object);
}

editor::Transaction::Transaction(const char* address, const Vector2i& value)
	: m_Address(address)
{
	json::Object object;
	object.SetObject();
	object.AddMember("x", json::Object(value.x), m_Document.GetAllocator());
	object.AddMember("y", json::Object(value.y), m_Document.GetAllocator());
	rapidjson::Pointer("").Set(m_Document, object);
}

editor::Transaction::Transaction(const char* address, const Vector2u& value)
	: m_Address(address)
{
	json::Object object;
	object.SetObject();
	object.AddMember("x", json::Object(value.x), m_Document.GetAllocator());
	object.AddMember("y", json::Object(value.y), m_Document.GetAllocator());
	rapidjson::Pointer("").Set(m_Document, object);
}

editor::Transaction::Transaction(const char* address, const Vector3f& value)
	: m_Address(address)
{
	json::Object object;
	object.SetObject();
	object.AddMember("x", json::Object(value.x), m_Document.GetAllocator());
	object.AddMember("y", json::Object(value.y), m_Document.GetAllocator());
	object.AddMember("z", json::Object(value.z), m_Document.GetAllocator());
	rapidjson::Pointer("").Set(m_Document, object);
}

editor::Transaction::Transaction(const char* address, const Vector3i& value)
	: m_Address(address)
{
	json::Object object;
	object.SetObject();
	object.AddMember("x", json::Object(value.x), m_Document.GetAllocator());
	object.AddMember("y", json::Object(value.y), m_Document.GetAllocator());
	object.AddMember("z", json::Object(value.z), m_Document.GetAllocator());
	rapidjson::Pointer("").Set(m_Document, object);
}

editor::Transaction::Transaction(const char* address, const str::String& value)
	: m_Address(address)
{
	rapidjson::Pointer("").Set(m_Document, value.c_str());
}

editor::Transaction::Transaction(const char* address, const json::Object& value)
	: m_Address(address)
{
	rapidjson::Pointer("").Set(m_Document, value);
}

void editor::Transaction::ApplyTo(json::Document& target) const
{
	// #note: we use m_Address here instead of "" because 
	// otherwise it would override the entire document
	rapidjson::Pointer(m_Address.c_str()).Set(target, m_Document);

	// #warning: m_Document is now invalidated
}