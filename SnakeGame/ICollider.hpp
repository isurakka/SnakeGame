#pragma once

#include <SFML/Graphics.hpp>
#include "Circle.hpp"

struct CollisionInfo
{
	CollisionInfo() :
		Collides(false),
		Inverse(false),
		Distance(0.0f),
		Normal(sf::Vector2f())
	{

	}

	CollisionInfo(bool collides, bool inverse, float distance, sf::Vector2f normal) :
		Collides(collides),
		Inverse(inverse),
		Distance(distance),
		Normal(normal)
	{ }

	bool Collides;
	bool Inverse;
	float Distance;
	sf::Vector2f Normal;
};

class ICollider
{
public:
	virtual ~ICollider(void) { };
	virtual CollisionInfo Collides(const Circle& circle) = 0;
};