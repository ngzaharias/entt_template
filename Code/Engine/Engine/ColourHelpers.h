#pragma once

#include <Engine/Colour.h>
#include <Engine/Vector3.h>

namespace colour
{
	Vector3f LerpRGB(const Vector3f& a, const Vector3f& b, const float t);

	Vector3f ToHSV(const Vector3f& rgb);
	Vector3f ToRGB(const Vector3f& hsv);
}
