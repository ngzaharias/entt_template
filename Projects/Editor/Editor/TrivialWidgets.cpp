#include "Editor/EditorPCH.h"
#include "Editor/TrivialWidgets.h"

#include <imgui/imgui.h>

void widget::TypeAsIs(bool& value)
{
	ImGui::Checkbox("", &value);
}

void widget::TypeAsIs(int32& value)
{
	ImGui::DragInt("", &value);
}

void widget::TypeAsIs(uint32& value)
{
	ImGui::DragScalar("", ImGuiDataType_U32, &value, 1.f, nullptr, nullptr, "%d", 0);
}

void widget::TypeAsIs(float& value)
{
	ImGui::DragFloat("", &value);
}

void widget::TypeAsIs(str::String& value)
{
	const size_t size = 256;
	char* buffer = new char[size];
	strncpy_s(buffer, size, value.c_str(), size);

	ImGui::InputText("", buffer, size);
	value = buffer;
}

void widget::TypeAsIs(Vector2f& value)
{
	ImGui::DragFloat2("", &value.x);
}

void widget::TypeAsIs(Vector2i& value)
{
	ImGui::DragInt2("", &value.x);
}

void widget::TypeAsIs(Vector2u& value)
{
	ImGui::DragScalarN("", ImGuiDataType_U32, &value.x, 2, 1.f, nullptr, nullptr, "%d", 0);
}

void widget::TypeAsIs(Vector3f& value)
{
	ImGui::DragFloat3("", &value.x);
}

void widget::TypeAsIs(Vector3i& value)
{
	ImGui::DragInt3("", &value.x);
}
