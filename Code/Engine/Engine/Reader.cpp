#include "EnginePCH.h"
#include "Engine/Reader.h"

serialize::Reader::Reader(const char* data)
{
	m_Document.Parse(data);
}

serialize::Reader::Reader(const char* data, bool isReplicating)
	: m_IsReplicating(isReplicating)
{
	m_Document.Parse(data);
}

void serialize::Reader::Visit(bool& value)
{
	value = m_Document[m_Index++].GetBool();
}

void serialize::Reader::Visit(int32& value)
{
	value = m_Document[m_Index++].GetInt();
}

void serialize::Reader::Visit(uint32& value)
{
	value = m_Document[m_Index++].GetUint();
}

void serialize::Reader::Visit(double& value)
{
	value = m_Document[m_Index++].GetDouble();
}

void serialize::Reader::Visit(float& value)
{
	value = static_cast<float>(m_Document[m_Index++].GetDouble());
}

void serialize::Reader::Visit(str::Name& value)
{
	str::String string;
	Visit(string);
	value = NAME(string);
}

void serialize::Reader::Visit(str::String& value)
{
	value = m_Document[m_Index++].GetString();
}

void serialize::Reader::Visit(Vector2f& value)
{
	value.x = static_cast<float>(m_Document[m_Index++].GetDouble());
	value.y = static_cast<float>(m_Document[m_Index++].GetDouble());
}

void serialize::Reader::Visit(Vector2i& value)
{
	value.x = m_Document[m_Index++].GetInt();
	value.y = m_Document[m_Index++].GetInt();
}

void serialize::Reader::Visit(Vector2u& value)
{
	value.x = m_Document[m_Index++].GetUint();
	value.y = m_Document[m_Index++].GetUint();
}

void serialize::Reader::Visit(Vector3f& value)
{
	value.x = static_cast<float>(m_Document[m_Index++].GetDouble());
	value.y = static_cast<float>(m_Document[m_Index++].GetDouble());
	value.z = static_cast<float>(m_Document[m_Index++].GetDouble());
}

void serialize::Reader::Visit(Vector3i& value)
{
	value.x = m_Document[m_Index++].GetInt();
	value.y = m_Document[m_Index++].GetInt();
	value.z = m_Document[m_Index++].GetInt();
}