#pragma once

#include <Engine/AttributeTypes.h>

namespace widget
{
	void TypeOverload(bool& value, const Attributes attributes = {});

	void TypeOverload(int32& value, const Attributes attributes = {});
	void TypeOverload(uint32& value, const Attributes attributes = {});

	void TypeOverload(float& value, const Attributes attributes = {});

	void TypeOverload(str::String& value, const Attributes attributes = {});

	void TypeOverload(Vector2f& value, const Attributes attributes = {});
	void TypeOverload(Vector2i& value, const Attributes attributes = {});
	void TypeOverload(Vector2u& value, const Attributes attributes = {});

	void TypeOverload(Vector3f& value, const Attributes attributes = {});
	void TypeOverload(Vector3i& value, const Attributes attributes = {});
}
