#include "EditorPCH.h"
#include "Editor/TrivialWidgets.h"

#include <imgui/imgui.h>

namespace
{
	template<typename Type>
	void Record(const Type& value, editor::InspectorInfo& info)
	{
		info.m_Transactions.emplace_back(info.m_Address, value);
	}
}

void editor::TypeOverload(bool& value, InspectorInfo& info)
{
	ImGui::Checkbox("", &value);
	
	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(float& value, InspectorInfo& info)
{
	const auto& range = info.m_Attributes.m_Range;
	const float min = range && range->m_Min ? *range->m_Min : -FLT_MAX;
	const float max = range && range->m_Max ? *range->m_Max : FLT_MAX;
	const float speed = 1.f;

	ImGui::DragFloat("", &value, speed, min, max);

	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(int32& value, InspectorInfo& info)
{
	const auto& range = info.m_Attributes.m_Range;
	const int32 min = range && range->m_Min ? static_cast<int32>(*range->m_Min) : -INT_MAX;
	const int32 max = range && range->m_Max ? static_cast<int32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragInt("", &value, speed, min, max);
	
	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(uint32& value, InspectorInfo& info)
{
	const auto& range = info.m_Attributes.m_Range;
	const uint32 min = range && range->m_Min ? static_cast<uint32>(*range->m_Min) : 0;
	const uint32 max = range && range->m_Max ? static_cast<uint32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragScalar("", ImGuiDataType_U32, &value, 1.f, nullptr, nullptr, "%d", 0);
	
	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(Vector2f& value, InspectorInfo& info)
{
	const auto& range = info.m_Attributes.m_Range;
	const float min = range && range->m_Min ? *range->m_Min : -FLT_MAX;
	const float max = range && range->m_Max ? *range->m_Max : FLT_MAX;
	const float speed = 1.f;

	ImGui::DragFloat2("", &value.x, speed, min, max);
	
	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(Vector2i& value, InspectorInfo& info)
{
	const auto& range = info.m_Attributes.m_Range;
	const int32 min = range && range->m_Min ? static_cast<int32>(*range->m_Min) : -INT_MAX;
	const int32 max = range && range->m_Max ? static_cast<int32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragInt2("", &value.x, speed, min, max);
	
	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(Vector2u& value, InspectorInfo& info)
{
	const auto& range = info.m_Attributes.m_Range;
	const uint32 min = range && range->m_Min ? static_cast<uint32>(*range->m_Min) : 0;
	const uint32 max = range && range->m_Max ? static_cast<uint32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragScalarN("", ImGuiDataType_U32, &value.x, 2, speed, &min, &max, "%d", 0);
	
	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(Vector3f& value, InspectorInfo& info)
{
	const auto& range = info.m_Attributes.m_Range;
	const float min = range && range->m_Min ? *range->m_Min : -FLT_MAX;
	const float max = range && range->m_Max ? *range->m_Max : FLT_MAX;
	const float speed = 1.f;

	ImGui::DragFloat3("", &value.x, speed, min, max);
	
	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(Vector3i& value, InspectorInfo& info)
{
	const auto& range = info.m_Attributes.m_Range;
	const int32 min = range && range->m_Min ? static_cast<int32>(*range->m_Min) : -INT_MAX;
	const int32 max = range && range->m_Max ? static_cast<int32>(*range->m_Max) : INT_MAX;
	const float speed = 1.f;

	ImGui::DragInt3("", &value.x, speed, min, max);

	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}

void editor::TypeOverload(str::String& value, InspectorInfo& info)
{
	const size_t size = 256;
	char* buffer = new char[size];
	strncpy_s(buffer, size, value.c_str(), size);

	const bool isChanged = ImGui::InputText("", buffer, size);
	value = buffer;

	if (ImGui::IsItemDeactivatedAfterEdit())
		Record(value, info);
}