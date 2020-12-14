#pragma once

struct ImVec2;

namespace widget
{
	constexpr float s_ArrowWidth = 14.f;

	bool ArrowButton(const char* str_id);

	bool CollapsingHeader(const char* fmt, ...);

	void GroupPanel_Begin(const char* name, const ImVec2& size);
	void GroupPanel_End();
}