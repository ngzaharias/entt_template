#include "Editor/EditorPCH.h"
#include "Editor/TrivialWidgets.h"

#include <imgui/imgui.h>

void widget::TypeOverload(bool& value, const Attributes attributes)
{
	ImGui::Checkbox("", &value);
}

void widget::TypeOverload(int32& value, const Attributes attributes)
{
	const auto& range = attributes.m_Range;
	const int32 min = range && range->m_Min ? static_cast<int32>(*range->m_Min) : -INT_MAX;
	const int32 max = range && range->m_Max ? static_cast<int32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragInt("", &value, speed, min, max);
}

void widget::TypeOverload(uint32& value, const Attributes attributes)
{
	const auto& range = attributes.m_Range;
	const uint32 min = range && range->m_Min ? static_cast<uint32>(*range->m_Min) : 0;
	const uint32 max = range && range->m_Max ? static_cast<uint32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragScalar("", ImGuiDataType_U32, &value, 1.f, nullptr, nullptr, "%d", 0);
}

void widget::TypeOverload(float& value, const Attributes attributes)
{
	const auto& range = attributes.m_Range;
	const float min = range && range->m_Min ? *range->m_Min : -FLT_MAX;
	const float max = range && range->m_Max ? *range->m_Max : FLT_MAX;
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
	const auto& range = attributes.m_Range;
	const float min = range && range->m_Min ? *range->m_Min : -FLT_MAX;
	const float max = range && range->m_Max ? *range->m_Max : FLT_MAX;
	const float speed = 1.f;

	ImGui::DragFloat2("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector2i& value, const Attributes attributes)
{
	const auto& range = attributes.m_Range;
	const int32 min = range && range->m_Min ? static_cast<int32>(*range->m_Min) : -INT_MAX;
	const int32 max = range && range->m_Max ? static_cast<int32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragInt2("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector2u& value, const Attributes attributes)
{
	const auto& range = attributes.m_Range;
	const uint32 min = range && range->m_Min ? static_cast<uint32>(*range->m_Min) : 0;
	const uint32 max = range && range->m_Max ? static_cast<uint32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragScalarN("", ImGuiDataType_U32, &value.x, 2, speed, &min, &max, "%d", 0);
}

void widget::TypeOverload(Vector3f& value, const Attributes attributes)
{
	const auto& range = attributes.m_Range;
	const float min = range && range->m_Min ? *range->m_Min : -FLT_MAX;
	const float max = range && range->m_Max ? *range->m_Max : FLT_MAX;
	const float speed = 1.f;

	ImGui::DragFloat3("", &value.x, speed, min, max);
}

void widget::TypeOverload(Vector3i& value, const Attributes attributes)
{
	const auto& range = attributes.m_Range;
	const int32 min = range && range->m_Min ? static_cast<int32>(*range->m_Min) : -INT_MAX;
	const int32 max = range && range->m_Max ? static_cast<int32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragInt3("", &value.x, speed, min, max);
}
