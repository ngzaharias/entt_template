#include "Editor/EditorPCH.h"
#include "Editor/TrivialWidgets.h"

#include <imgui/imgui.h>

void widget::TypeOverload(bool& value, const Attributes attributes)
{
	ImGui::Checkbox("", &value);
}

void widget::TypeOverload(int32& value, const Attributes attributes)
{
	const int32 min = attributes.m_Range ? static_cast<int32>(attributes.m_Range->m_Min) : 0;
	const int32 max = attributes.m_Range ? static_cast<int32>(attributes.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragInt("", &value, speed, min, max);
}

void widget::TypeOverload(uint32& value, const Attributes attributes)
{
	const uint32 min = attributes.m_Range ? static_cast<uint32>(attributes.m_Range->m_Min) : 0;
	const uint32 max = attributes.m_Range ? static_cast<uint32>(attributes.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragScalar("", ImGuiDataType_U32, &value, 1.f, nullptr, nullptr, "%d", 0);
}

void widget::TypeOverload(float& value, const Attributes attributes)
{
	const float min = attributes.m_Range ? attributes.m_Range->m_Min : 0.f;
	const float max = attributes.m_Range ? attributes.m_Range->m_Max : 0.f;
	const float speed = 1.f;

	ImGui::DragFloat("", &value, speed, min, max);
}

void widget::TypeOverload(str::String& value, const Attributes attributes)
{
	const size_t size = 256;
	char* buffer = new char[size];
	strncpy_s(buffer, size, value.c_str(), size);

	ImGui::InputText("", buffer, size);
	value = buffer;
}

void widget::TypeOverload(Vector2f& value, const Attributes attributes)
{
	const float min = attributes.m_Range ? attributes.m_Range->m_Min : 0.f;
	const float max = attributes.m_Range ? attributes.m_Range->m_Max : 0.f;
	const float speed = 1.f;

	ImGui::DragFloat2("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector2i& value, const Attributes attributes)
{
	const int32 min = attributes.m_Range ? static_cast<int32>(attributes.m_Range->m_Min) : 0;
	const int32 max = attributes.m_Range ? static_cast<int32>(attributes.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragInt2("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector2u& value, const Attributes attributes)
{
	const uint32 min = attributes.m_Range ? static_cast<uint32>(attributes.m_Range->m_Min) : 0;
	const uint32 max = attributes.m_Range ? static_cast<uint32>(attributes.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragScalarN("", ImGuiDataType_U32, &value.x, 2, speed, &min, &max, "%d", 0);
}

void widget::TypeOverload(Vector3f& value, const Attributes attributes)
{
	const float min = attributes.m_Range ? attributes.m_Range->m_Min : 0.f;
	const float max = attributes.m_Range ? attributes.m_Range->m_Max : 0.f;
	const float speed = 1.f;

	ImGui::DragFloat3("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector3i& value, const Attributes attributes)
{
	const int32 min = attributes.m_Range ? static_cast<int32>(attributes.m_Range->m_Min) : 0;
	const int32 max = attributes.m_Range ? static_cast<int32>(attributes.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragInt3("", &value.x, speed, min, max);
}
