#include "EnginePCH.h"
#include "Engine/Writer.h"

serialize::Writer::Writer()
	: m_Writer(m_Buffer)
{
	m_Writer.StartArray();
}

serialize::Writer::Writer(Mode mode)
	: m_Mode(mode)
	, m_Writer(m_Buffer)
{
	m_Writer.StartArray();
}

str::StringView serialize::Writer::Conclude()
{
	m_Writer.EndArray();
	return m_Buffer.GetString();
}

void serialize::Writer::Visit(const bool& value)
{
	m_Writer.Bool(value);
}

void serialize::Writer::Visit(const int32& value)
{
	m_Writer.Int(value);
}

void serialize::Writer::Visit(const uint32& value)
{
	m_Writer.Uint(value);
}

void serialize::Writer::Visit(const double& value)
{
	m_Writer.Double(value);
}

void serialize::Writer::Visit(const float& value)
{
	m_Writer.Double(value);
}

void serialize::Writer::Visit(const str::Name& value)
{
	Visit(value.ToString());
}

void serialize::Writer::Visit(const str::String& value)
{
	const auto length = static_cast<rapidjson::SizeType>(value.length());
	m_Writer.String(value.c_str(), length, true);
}

void serialize::Writer::Visit(const Vector2f& value)
{
	m_Writer.Double(value.x);
	m_Writer.Double(value.y);
}

void serialize::Writer::Visit(const Vector2i& value)
{
	m_Writer.Int(value.x);
	m_Writer.Int(value.y);
}

void serialize::Writer::Visit(const Vector2u& value)
{
	m_Writer.Uint(value.x);
	m_Writer.Uint(value.y);
}

void serialize::Writer::Visit(const Vector3f& value)
{
	m_Writer.Double(value.x);
	m_Writer.Double(value.y);
	m_Writer.Double(value.z);
}

void serialize::Writer::Visit(const Vector3i& value)
{
	m_Writer.Int(value.x);
	m_Writer.Int(value.y);
	m_Writer.Int(value.z);
}