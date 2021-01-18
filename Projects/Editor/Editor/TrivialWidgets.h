#pragma once

namespace widget
{
	void TypeAsIs(bool& value);

	void TypeAsIs(int32& value);
	void TypeAsIs(uint32& value);

	void TypeAsIs(float& value);

	void TypeAsIs(str::String& value);

	void TypeAsIs(Vector2f& value);
	void TypeAsIs(Vector2i& value);
	void TypeAsIs(Vector2u& value);

	void TypeAsIs(Vector3f& value);
	void TypeAsIs(Vector3i& value);
}
