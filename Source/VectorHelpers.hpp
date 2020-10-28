#pragma once

#include <SFML/System/Vector2.hpp>

float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

float Length(const sf::Vector2f& value);

[[nodiscard]]
sf::Vector2f Normalized(const sf::Vector2f& value);

sf::Vector2f Reflect(const sf::Vector2f& vector, const sf::Vector2f& normal);

sf::Vector2f Divide(const sf::Vector2f& lhs, const sf::Vector2f& rhs);

sf::Vector2f Multiply(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
