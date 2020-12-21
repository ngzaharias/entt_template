#pragma once

struct ImVec2;

namespace imgui
{
	constexpr float s_ArrowWidth = 14.f;

	bool ArrowButton(const char* str_id);

	void Bullet();

	bool FieldHeader(const char* fmt, ...);

	void GroupPanel_Begin(const char* name, const ImVec2& size);
	void GroupPanel_End();
}