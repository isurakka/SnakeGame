#include "CircleObstacle.hpp"


CircleObstacle::CircleObstacle(sf::Vector2f pos, float radius, bool inverse) : 
	Inverse(inverse),
	Color(sf::Color::White),
	BackgroundColor(sf::Color::Black)
{
	Position = pos;
	Radius = radius;
}


CircleObstacle::~CircleObstacle()
{

}

CollisionInfo CircleObstacle::Collides(const Circle& circle)
{
	sf::Vector2f sub = Position - circle.GetPosition();

	bool test;
	if (!Inverse)
		test = thor::length(sub) < Radius + circle.GetRadius();
	else
		test = thor::length(sub) > Radius - circle.GetRadius();

	float distance;
	if (!Inverse)
		distance = thor::length(sub) - Radius - circle.GetRadius();
	else
		distance = Radius - thor::length(sub) - circle.GetRadius();

	distance = fabsf(distance);
		
	sf::Vector2f normal = sub != sf::Vector2f() ? thor::unitVector(sub) : sf::Vector2f();
	if (Inverse)
		normal = normal;
	
	CollisionInfo info(
		test, 
		Inverse, 
		distance,
		normal);

	return info;
}

void CircleObstacle::Draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	sf::CircleShape circle(Radius, std::sqrtf(Radius * 12.0f));
	circle.setOrigin(Radius, Radius);
	circle.setPosition(Position);

	sf::Color color = Inverse ? BackgroundColor : Color;
	circle.setFillColor(color);

	//circle.setOutlineColor(sf::Color(255, 255, 255, 100));

	target.draw(circle);
}