#include "PlayerWorm.hpp"


PlayerWorm::PlayerWorm(sf::Vector2f position) : 
	Worm(position), 
	turnAcc(0.0f), 
	turnInterval(0.05f)
{

}


PlayerWorm::~PlayerWorm(void)
{

}

void PlayerWorm::OnEat(int score)
{
	Worm::OnEat(score);

	MaxSpeed += (float)score / 1000.0f;
}

void PlayerWorm::Tick(float step, int frame)
{
	turnAcc += step;

	while (turnAcc >= turnInterval)
	{
		turnAcc -= turnInterval;

		const float rotAmount = 0.23f * (turnInterval / step);

		bool turned = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			thor::rotate(Direction, -rotAmount);
			turned = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			thor::rotate(Direction, rotAmount);
			turned = true;
		}

		if (turned)
		{
			Speed -= rotAmount / 600.0f;
			if (Speed < 0.0f)
				Speed = 0.0f;
		}
	}

	Worm::Tick(step, frame);
};
