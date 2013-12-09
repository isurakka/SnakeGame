#pragma once

#include "Worm.hpp"

class PlayerWorm : public Worm
{
public:
	PlayerWorm(sf::Vector2f position = sf::Vector2f());
	~PlayerWorm(void);
	virtual void Tick(float step, int frame);
	virtual void OnEat(int score);
private:
	float turnInterval;
	float turnAcc;
};

