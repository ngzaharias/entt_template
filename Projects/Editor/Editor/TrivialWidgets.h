#pragma once

#include <Engine/PropertyTypes.h>

namespace widget
{
	void TypeOverload(bool& value, const Properties properties = {});

	void TypeOverload(int32& value, const Properties properties = {});
	void TypeOverload(uint32& value, const Properties properties = {});

	void TypeOverload(float& value, const Properties properties = {});

	void TypeOverload(str::String& value, const Properties properties = {});

	void TypeOverload(Vector2f& value, const Properties properties = {});
	void TypeOverload(Vector2i& value, const Properties properties = {});
	void TypeOverload(Vector2u& value, const Properties properties = {});

	void TypeOverload(Vector3f& value, const Properties properties = {});
	void TypeOverload(Vector3i& value, const Properties properties = {});
}
