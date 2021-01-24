#pragma once

struct ImVec2;

namespace imgui
{
	constexpr float s_ArrowWidth = 14.f;

	bool ArrowButton(const char* str_id);

	void Bullet();

	bool FakeCombo(const char* label, const char* text);

	bool InspectHeader(const char* fmt, ...);
}