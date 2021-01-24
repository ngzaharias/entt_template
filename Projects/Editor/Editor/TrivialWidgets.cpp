#include "Editor/EditorPCH.h"
#include "Editor/TrivialWidgets.h"

#include <imgui/imgui.h>

void widget::TypeOverload(bool& value, const Properties properties)
{
	ImGui::Checkbox("", &value);
}

void widget::TypeOverload(int32& value, const Properties properties)
{
	const int32 min = properties.m_Range ? static_cast<int32>(properties.m_Range->m_Min) : 0;
	const int32 max = properties.m_Range ? static_cast<int32>(properties.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragInt("", &value, speed, min, max);
}

void widget::TypeOverload(uint32& value, const Properties properties)
{
	const uint32 min = properties.m_Range ? static_cast<uint32>(properties.m_Range->m_Min) : 0;
	const uint32 max = properties.m_Range ? static_cast<uint32>(properties.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragScalar("", ImGuiDataType_U32, &value, 1.f, nullptr, nullptr, "%d", 0);
}

void widget::TypeOverload(float& value, const Properties properties)
{
	const float min = properties.m_Range ? properties.m_Range->m_Min : 0.f;
	const float max = properties.m_Range ? properties.m_Range->m_Max : 0.f;
	const float speed = 1.f;

	ImGui::DragFloat("", &value, speed, min, max);
}

void widget::TypeOverload(str::String& value, const Properties properties)
{
	const size_t size = 256;
	char* buffer = new char[size];
	strncpy_s(buffer, size, value.c_str(), size);

	ImGui::InputText("", buffer, size);
	value = buffer;
}

void widget::TypeOverload(Vector2f& value, const Properties properties)
{
	const float min = properties.m_Range ? properties.m_Range->m_Min : 0.f;
	const float max = properties.m_Range ? properties.m_Range->m_Max : 0.f;
	const float speed = 1.f;

	ImGui::DragFloat2("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector2i& value, const Properties properties)
{
	const int32 min = properties.m_Range ? static_cast<int32>(properties.m_Range->m_Min) : 0;
	const int32 max = properties.m_Range ? static_cast<int32>(properties.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragInt2("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector2u& value, const Properties properties)
{
	const uint32 min = properties.m_Range ? static_cast<uint32>(properties.m_Range->m_Min) : 0;
	const uint32 max = properties.m_Range ? static_cast<uint32>(properties.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragScalarN("", ImGuiDataType_U32, &value.x, 2, speed, &min, &max, "%d", 0);
}

void widget::TypeOverload(Vector3f& value, const Properties properties)
{
	const float min = properties.m_Range ? properties.m_Range->m_Min : 0.f;
	const float max = properties.m_Range ? properties.m_Range->m_Max : 0.f;
	const float speed = 1.f;

	ImGui::DragFloat3("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector3i& value, const Properties properties)
{
	const int32 min = properties.m_Range ? static_cast<int32>(properties.m_Range->m_Min) : 0;
	const int32 max = properties.m_Range ? static_cast<int32>(properties.m_Range->m_Max) : 0;
	const float speed = 1.f;

	ImGui::DragInt3("", &value.x, speed, min, max);
}
