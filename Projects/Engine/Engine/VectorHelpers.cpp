#include "Engine/VectorHelpers.h"

#include <cmath>

float Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

float Length(const sf::Vector2f& value)
{
	return std::sqrt(value.x * value.x + value.y * value.y);
}

sf::Vector2f Normalized(const sf::Vector2f& value)
{
	//#todo: handle (0,0) vector
	return value / Length(value);
}

sf::Vector2f Reflect(const sf::Vector2f& vector, const sf::Vector2f& normal)
{
	// -2 * (V dot N)*N + V
	const float dot2 = Dot(vector, normal) * -2.0f;
	return (dot2 * normal) + vector;
}

sf::Vector2f Divide(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

sf::Vector2f Multiply(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}
