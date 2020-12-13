#pragma once

struct ImVec2;

namespace widget
{
	bool CollapsingHeader(const char* fmt, ...);

	void GroupPanel_Begin(const char* name, const ImVec2& size);
	void GroupPanel_End();
}