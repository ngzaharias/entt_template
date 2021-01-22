#include "Helpers.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <algorithm>
//
//float imgui::GetColumnOffset(const char* str_id, int count, int index)
//{
//	ImGuiWindow* window = ImGui::GetCurrentWindow();
//	if (!window)
//		return 0.f;
//
//	ImGuiID id = ImGui::GetColumnsID(str_id, count);
//	ImGuiColumns* columns = ImGui::FindOrCreateColumns(window, id);
//	if (!columns)
//		return 0.f;
//
//	index = std::clamp(index, 0, columns->Columns.Size - 1);
//	const float t = columns->Columns[index].OffsetNorm;
//	const float x_offset = ImLerp(columns->OffMinX, columns->OffMaxX, t);
//	return x_offset;
//}
//
//void imgui::SetColumnIndex(int index)
//{
//	ImGuiWindow* window = ImGui::GetCurrentWindow();
//	if (window->SkipItems || window->DC.CurrentColumns == NULL)
//		return;
//
//	ImGuiContext& g = *GImGui;
//	ImGuiColumns* columns = window->DC.CurrentColumns;
//
//	if (columns->Count == 1)
//	{
//		window->DC.CursorPos.x = IM_FLOOR(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);
//		IM_ASSERT(columns->Current == 0);
//		return;
//	}
//
//	// set column
//	index = std::clamp(index, 0, window->DC.CurrentColumns->Count - 1);
//	window->DC.CurrentColumns->Current = index;
//
//	ImGui::PopItemWidth();
//
//	// Optimization: avoid PopClipRect() + SetCurrentChannel() + PushClipRect()
//	// (which would needlessly attempt to update commands in the wrong channel, then pop or overwrite them),
//	ImGuiColumnData* column = &columns->Columns[columns->Current];
//	ImGui::SetWindowClipRectBeforeSetChannel(window, column->ClipRect);
//	columns->Splitter.SetCurrentChannel(window->DrawList, columns->Current + 1);
//
//	const float column_padding = g.Style.ItemSpacing.x;
//	columns->LineMaxY = ImMax(columns->LineMaxY, window->DC.CursorPos.y);
//	if (columns->Current > 0)
//	{
//		// Columns 1+ ignore IndentX (by canceling it out)
//		// FIXME-COLUMNS: Unnecessary, could be locked?
//		window->DC.ColumnsOffset.x = ImGui::GetColumnOffset(columns->Current) - window->DC.Indent.x + column_padding;
//	}
//	else
//	{
//		// New row/line: column 0 honor IndentX.
//		window->DC.ColumnsOffset.x = ImMax(/*column_padding - */window->WindowPadding.x, 0.0f);
//		columns->LineMinY = columns->LineMaxY;
//	}
//	window->DC.CursorPos.x = IM_FLOOR(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);
//	window->DC.CursorPos.y = columns->LineMinY;
//	window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);
//	window->DC.CurrLineTextBaseOffset = 0.0f;
//
//	// FIXME-COLUMNS: Share code with BeginColumns() - move code on columns setup.
//	float offset_0 = ImGui::GetColumnOffset(columns->Current);
//	float offset_1 = ImGui::GetColumnOffset(columns->Current + 1);
//	float width = offset_1 - offset_0;
//	ImGui::PushItemWidth(width * 0.65f);
//	window->WorkRect.Max.x = window->Pos.x + offset_1 - column_padding;
//}