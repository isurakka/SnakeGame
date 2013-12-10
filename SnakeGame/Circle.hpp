#pragma once

#include <SFML/Graphics.hpp>

class Circle
{
public:
	Circle()
	{
		
	}
	virtual ~Circle(void) {  };
	virtual sf::Vector2f GetPosition() const = 0;
	virtual void SetPosition(sf::Vector2f pos) = 0;
	virtual float GetRadius() const = 0;
	virtual void SetRadius(float rad) = 0;
	sf::FloatRect GetRect() // Get AABB of the circle.
	{
		return sf::FloatRect(
			GetPosition().x - GetRadius(), 
			GetPosition().y - GetRadius(), 
			GetRadius() * 2.0f, 
			GetRadius() * 2.0f);
	};
};

