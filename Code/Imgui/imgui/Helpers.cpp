#include "Helpers.h"

#include "imgui/imgui.h"

void imgui::InspectorBegin()
{
	ImGui::BeginTable("Inspector", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable);
	ImGui::TableNextRow();
	//ImGui::TableSetColumnIndex(0);
	//ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
	ImGui::TableSetColumnIndex(1);
	ImGui::PushItemWidth(-FLT_MIN);
}

void imgui::InspectorEnd()
{
	ImGui::EndTable();
}