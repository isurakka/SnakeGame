#include "Food.hpp"


Food::Food(sf::Vector2f pos, int layers) : 
	Position(pos),
	InnerRadius(13.0f),
	Points(10),
	Color(sf::Color::Green),
	LayerColor(sf::Color::Blue),
	Layers(layers),
	LayerRadiusAdd(10.0f)
{
	Radius = InnerRadius + layers * LayerRadiusAdd;
	Points += layers * Points;
}

Food::~Food(void)
{

}

bool Food::RemoveLayer()
{
	if (Layers > 0)
	{
		Layers--;
		Radius -= LayerRadiusAdd;
		return true;
	}
	return false;
}

CollisionInfo Food::Collides(const Circle& circle)
{
	CollisionInfo info;

	sf::Vector2f sub = Position - circle.GetPosition();

	bool test = thor::length(sub) < Radius + circle.GetRadius();

	info.Collides = test;
	info.Distance = thor::length(sub) - Radius - circle.GetRadius();
	info.Normal = thor::length(sub) > 0 ? thor::unitVector(-sub) : sf::Vector2f();

	return info;
}

void Food::Draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (int i = Layers - 1; i > -1; i--)
	{
		float radius = InnerRadius + (i + 1) * LayerRadiusAdd;

		sf::CircleShape circle(radius, radius * 2);
		circle.setOrigin(radius, radius);
		circle.setPosition(Position);
		circle.setFillColor(thor::blendColors(Color, LayerColor, (float)(i + 1) / (float)Layers));
		target.draw(circle);
	}

	sf::CircleShape circle(InnerRadius, InnerRadius * 2);
	circle.setOrigin(InnerRadius, InnerRadius);
	circle.setPosition(Position);
	circle.setFillColor(Color);
	target.draw(circle);
};
