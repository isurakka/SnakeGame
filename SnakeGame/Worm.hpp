#pragma once

#include <SFML/Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>
#include "Thor\Vectors.hpp"
#include <iostream>

#include "GameObject.hpp"
#include "Map.hpp"

class Worm : public GameObject, public Circle, public ICollider
{
public:
	Worm(sf::Vector2f position = sf::Vector2f());
	~Worm(void);
	virtual void Tick(float step, int frame);
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates& states);
	float Speed;
	float MaxSpeed;
	float Acceleration;
	float MaxLength;
	float Thickness;
	sf::Vector2f Direction;
	sf::Color Color;
	float GetLength();
	int Score;
	sf::Vector2f GetHeadPos() const;
	sf::Vector2f GetLastHeadPos();
	virtual void OnEat(int score);
	std::vector<sf::Vector2f> GetCollisionPoints();
	float Multiplier;
	int Combo;
	float ComboTime;
	float ComboLeft;
	bool OnCollision(CollisionInfo info, GameObject* hit);
	virtual CollisionInfo Collides(const Circle& circle);

	virtual sf::Vector2f GetPosition() const { return GetHeadPos(); };
	virtual void SetPosition(sf::Vector2f pos) { addPoint(pos); };
	virtual float GetRadius() const { return Thickness / 2.0f; };
	virtual void SetRadius(float rad) { Thickness = rad * 2.0f; };
protected:
	float multAdd;
	float multTime;
	float multLeft;
	float bounceMult;
	float bounceAdd;
	float bounceTime;
	float bounceLeft;
	void addPoint(sf::Vector2f point);
	void trimPoints(std::vector<sf::Vector2f>& p, float length);
private:
	sf::Vector2f lastHead;
	std::vector<sf::Vector2f> points;
	void drawPoints(sf::RenderTarget& target, sf::Vector2f& p1, sf::Vector2f& p2, bool drawP1End = false);

	sf::SoundBuffer* foodBuffer;
	sf::Sound foodSound;

	sf::SoundBuffer* bounceBuffer;
	sf::Sound bounceSound;
};

