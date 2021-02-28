#pragma once

#include <Editor/InspectorTypes.h>

namespace editor
{
	void TypeOverload(bool& value, InspectorInfo& info);
	void TypeOverload(float& value, InspectorInfo& info);
	void TypeOverload(int32& value, InspectorInfo& info);
	void TypeOverload(uint32& value, InspectorInfo& info);
	void TypeOverload(Vector2f& value, InspectorInfo& info);
	void TypeOverload(Vector2i& value, InspectorInfo& info);
	void TypeOverload(Vector2u& value, InspectorInfo& info);
	void TypeOverload(Vector3f& value, InspectorInfo& info);
	void TypeOverload(Vector3i& value, InspectorInfo& info);
	void TypeOverload(str::String& value, InspectorInfo& info);
}
