#include <Core/Math.h>

inline float Vector2f::Length() const
{
	return math::Sqrt(x * x + y * y);
}

inline void Vector2f::Normalize()
{
	const float length = Length();
	if (length > KINDA_SMALL_FLOAT)
	{
		*this *= 1.f / length;
	}
	else
	{
		x = y = 0.f;
	}
}

inline void Vector2f::NormalizeUnsafe()
{
	*this *= 1.f / Length();
}

inline Vector2f Vector2f::Normalized() const
{
	Vector2f value(*this);
	value.Normalize();
	return value;
}

inline Vector2f Vector2f::NormalizedUnsafe() const
{
	Vector2f value(*this);
	value.NormalizeUnsafe();
	return value;
}

inline Vector3f Vector2f::X0Y() const
{
	return Vector3f(x, 0.f, y);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

inline float Vector3f::Length() const
{
	return math::Sqrt(x * x + y * y + z * z);
}

inline void Vector3f::Normalize()
{
	const float length = Length();
	if (length > KINDA_SMALL_FLOAT)
	{
		*this *= 1.f / length;
	}
	else
	{
		x = y = z = 0.f;
	}
}

inline void Vector3f::NormalizeUnsafe()
{
	*this *= 1.f / Length();
}

inline Vector3f Vector3f::Normalized() const
{
	Vector3f value(*this);
	value.Normalize();
	return value;
}

inline Vector3f Vector3f::NormalizedUnsafe() const
{
	Vector3f value(*this);
	value.NormalizeUnsafe();
	return value;
}

inline Vector2f Vector3f::XY() const
{
	return Vector2f(x, y);
}

inline Vector2f Vector3f::XZ() const
{
	return Vector2f(x, z);
}

inline Vector3f Vector3f::X0Z() const
{
	return Vector3f(x, 0.f, z);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

inline float Vector4f::Length() const
{
	return math::Sqrt(x * x + y * y + z * z + w * w);
}

inline void Vector4f::Normalize()
{
	const float length = Length();
	if (length > KINDA_SMALL_FLOAT)
	{
		*this *= 1.f / length;
	}
	else
	{
		x = y = z = w = 0.f;
	}
}

inline void Vector4f::NormalizeUnsafe()
{
	*this *= 1.f / Length();
}

inline Vector4f Vector4f::Normalized() const
{
	Vector4f value(*this);
	value.Normalize();
	return value;
}

inline Vector4f Vector4f::NormalizedUnsafe() const
{
	Vector4f value(*this);
	value.NormalizeUnsafe();
	return value;
}
