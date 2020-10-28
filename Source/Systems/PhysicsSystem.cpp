#include "PhysicsSystem.hpp"

#include "Math.hpp"
#include "VectorHelpers.hpp"

#include "Components/Collider.hpp"
#include "Components/Name.hpp"
#include "Components/Rigidbody.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

physics::PhysicsSystem::PhysicsSystem()
{
}

physics::PhysicsSystem::~PhysicsSystem()
{
}

void physics::PhysicsSystem::Update(entt::registry& registry, const sf::Time& time)
{
	const float deltaTime = Math::Min(0.1f, time.asSeconds());

	const auto viewA = registry.view<core::Transform, physics::Collider, physics::Rigidbody, physics::Velocity>();
	const auto viewB = registry.view<core::Transform, physics::Collider>();

	for (const entt::entity& entityA : viewA)
	{
		auto& colliderA = viewA.get<physics::Collider>(entityA);
		auto& nameA = registry.get<debug::Name>(entityA);
		auto& transformA = viewA.get<core::Transform>(entityA);
		auto& velocityA = viewA.get<physics::Velocity>(entityA);

		transformA.m_Translate += velocityA.m_Velocity * deltaTime;

		for (const entt::entity& entityB : viewB)
		{
			auto& colliderB = viewB.get<physics::Collider>(entityB);
			auto& nameB = registry.get<debug::Name>(entityB);
			auto& transformB = viewB.get<core::Transform>(entityB);

			if (entityA != entityB)
			{
				sf::Vector2f extentsA = Multiply(colliderA.m_Extents, transformA.m_Scale);
				sf::Vector2f extentsB = Multiply(colliderB.m_Extents, transformB.m_Scale);

				sf::FloatRect intersection;
				sf::FloatRect rectangleA = { transformA.m_Translate - extentsA, extentsA * 2.f };
				sf::FloatRect rectangleB = { transformB.m_Translate - extentsB, extentsB * 2.f };
				if (rectangleA.intersects(rectangleB, intersection))
				{
					// distance of box 'b2' to face on 'left' side of 'b1'.
					// distance of box 'b2' to face on 'right' side of 'b1'
					// distance of box 'b2' to face on 'top' side of 'b1'.
					// distance of box 'b2' to face on 'bottom' side of 'b1'.
					const float left = (rectangleB.left + rectangleB.width) - rectangleA.left;
					const float right = (rectangleA.left + rectangleA.width) - rectangleB.left;
					const float top = (rectangleA.top + rectangleA.height) - rectangleB.top;
					const float bottom = (rectangleB.top + rectangleB.height) - rectangleA.top;

					// #hack: move rigidbody out of the collider
					const sf::Vector2f direction = Normalized(velocityA.m_Velocity);
					const sf::Vector2f offset = { direction.x * intersection.width, direction.y * intersection.height };
					transformA.m_Translate -= offset;

					// #hack: not exactly the best way to get a hit normal
					sf::Vector2f normal;
					const float minX = Math::Min<float>(left, right);
					const float minY = Math::Min<float>(top, bottom);
					normal.x = (minX < minY) ? (left < right) ? -1.0f : 1.0f : 0.0f;
					normal.y = (minX < minY) ? 0.0f : (top < bottom) ? -1.0f : 1.0f;

					velocityA.m_Velocity = Reflect(velocityA.m_Velocity, normal);

					m_OnCollisionSignal.publish(entityA, entityB);
				}
			}
		}
	}
}
