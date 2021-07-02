#include "EnginePCH.h"
#include "Engine/ColourHelpers.h"

Vector3f colour::LerpRGB(const Vector3f& a, const Vector3f& b, const float t)
{
	const Vector3f hsvA = ToHSV(a);
	const Vector3f hsvB = ToHSV(b);
	const Vector3f hsvResult = math::Lerp(hsvA, hsvB, t);
	return ToRGB(hsvResult);
}

Vector3f colour::ToHSV(const Vector3f& rgb)
{
	Vector3f hsv;
	float mrgb, max, delta;

	mrgb = rgb.x < rgb.y ? rgb.x : rgb.y;
	mrgb = mrgb < rgb.z ? mrgb : rgb.z;

	max = rgb.x > rgb.y ? rgb.x : rgb.y;
	max = max > rgb.z ? max : rgb.z;

	hsv.z = max; // v
	delta = max - mrgb;
	if (delta < 0.00001f)
	{
		hsv.y = 0;
		hsv.x = 0; // undefrgbed, maybe nan?
		return hsv;
	}
	// #note: if Max is == 0, this divide would cause a crash
	if (max > 0.f)
	{
		hsv.y = (delta / max); // s
	}
	else
	{
		// if max is 0, then r = g = b = 0
		// s = 0, h is undefrgbed
		hsv.y = 0.f;
		hsv.x = 0.f;
		return hsv;
	}
	if (rgb.x >= max) // > is bogus, just keeps compilor happy
	{
		hsv.x = (rgb.y - rgb.z) / delta; // between yellow & magenta
	}
	else
	{
		if (rgb.y >= max)
		{
			hsv.x = 2.f + (rgb.z - rgb.x) / delta; // between cyan & yellow
		}
		else
		{
			hsv.x = 4.f + (rgb.x - rgb.y) / delta; // between magenta & cyan
		}
	}

	hsv.x *= 60.f; // degrees

	if (hsv.x < 0.f)
		hsv.x += 360.f;
	return hsv;
}

Vector3f colour::ToRGB(const Vector3f& hsv)
{
	float hh, p, q, t, ff;
	long i;
	Vector3f rgb;

	// < is bogus, just shuts up warnhsvgs
	if (hsv.y <= 0.f)
	{
		rgb.x = hsv.z;
		rgb.y = hsv.z;
		rgb.z = hsv.z;
		return rgb;
	}
	hh = hsv.x;
	if (hh >= 360.f) hh = 0.f;
	hh /= 60.f;
	i = (long)hh;
	ff = hh - i;
	p = hsv.z * (1.f - hsv.y);
	q = hsv.z * (1.f - (hsv.y * ff));
	t = hsv.z * (1.f - (hsv.y * (1.f - ff)));

	switch (i)
	{
	case 0:
		rgb.x = hsv.z;
		rgb.y = t;
		rgb.z = p;
		break;
	case 1:
		rgb.x = q;
		rgb.y = hsv.z;
		rgb.z = p;
		break;
	case 2:
		rgb.x = p;
		rgb.y = hsv.z;
		rgb.z = t;
		break;

	case 3:
		rgb.x = p;
		rgb.y = q;
		rgb.z = hsv.z;
		break;
	case 4:
		rgb.x = t;
		rgb.y = p;
		rgb.z = hsv.z;
		break;
	case 5:
	default:
		rgb.x = hsv.z;
		rgb.y = p;
		rgb.z = q;
		break;
	}
	return rgb;
}
