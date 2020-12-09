#pragma once

float Dot(const Vector2f& a, const Vector2f& b);

float Length(const Vector2f& value);

[[nodiscard]]
Vector2f Normalized(const Vector2f& value);

Vector2f Reflect(const Vector2f& vector, const Vector2f& normal);

Vector2f Divide(const Vector2f& lhs, const Vector2f& rhs);

Vector2f Multiply(const Vector2f& lhs, const Vector2f& rhs);
