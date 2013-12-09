#pragma once

#include "Thor\Vectors.hpp"

#include "GameObject.hpp"
#include "ICollider.hpp"
#include "Circle.hpp"

class CircleObstacle : public ICollider, public Circle, public GameObject
{
public:
	CircleObstacle(sf::Vector2f pos, float radius, bool inverse = false);
	~CircleObstacle();
	virtual CollisionInfo Collides(const Circle& circle);
	//virtual CollisionInfo Collides(sf::Vector2f point, float radius);
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates& states);
	bool Inverse;
	sf::Color Color;
	sf::Color BackgroundColor;

	sf::Vector2f Position;
	virtual sf::Vector2f GetPosition() const { return Position; };
	virtual void SetPosition(sf::Vector2f pos) { Position = pos; };

	float Radius;
	virtual float GetRadius() const { return Radius; };
	virtual void SetRadius(float rad) { Radius = rad; };
};

