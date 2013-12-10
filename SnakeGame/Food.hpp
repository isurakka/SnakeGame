#pragma once

#include <SFML/Graphics.hpp>
#include "Thor\Vectors.hpp"
#include "Thor\Graphics.hpp"

#include "GameObject.hpp"
#include "ICollider.hpp"
#include "Circle.hpp"

class Food : public GameObject, public ICollider, public Circle
{
public:
	Food(sf::Vector2f pos, int layers = 0);
	~Food(void);
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates& states);
	int Points;
	sf::Color Color;
	sf::Color LayerColor; // Outer layer colors

	virtual CollisionInfo Collides(const Circle& circle);

	bool RemoveLayer();

	float InnerRadius;
	float LayerRadiusAdd;
	int Layers;

	sf::Vector2f Position;
	virtual sf::Vector2f GetPosition() const { return Position; };
	virtual void SetPosition(sf::Vector2f pos) { Position = pos; };

	float Radius;
	virtual float GetRadius() const { return Radius; };
	virtual void SetRadius(float rad) { Radius = rad; };
};

