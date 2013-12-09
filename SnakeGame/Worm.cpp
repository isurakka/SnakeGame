#include "Worm.hpp"


Worm::Worm(sf::Vector2f position) : 
	Thickness(20.0f),
	Acceleration(0.0003f),
	MaxSpeed(0.1f), 
	Speed(0.0f),
	MaxLength(10.0f), 
	Direction(sf::Vector2f(1.0f, 0.0f)), 
	Color(sf::Color::White), 
	Score(0),
	Multiplier(1.0f),
	multAdd(0.1f),
	multTime(2.0f),
	multLeft(0.0f),
	bounceMult(1.0f),
	bounceAdd(0.015f),
	bounceTime(0.4f),
	bounceLeft(0.0f),
	Combo(0),
	ComboTime(5.0f),
	ComboLeft(0.0f)
{
	addPoint(position);

	foodBuffer = new sf::SoundBuffer();
	foodBuffer->loadFromFile("assets/food.wav");
	foodSound.setBuffer(*foodBuffer);

	bounceBuffer = new sf::SoundBuffer();
	bounceBuffer->loadFromFile("assets/bounce.wav");
	bounceSound.setBuffer(*bounceBuffer);
}

Worm::~Worm(void)
{

}

float Worm::GetLength()
{
	float len = 0.0f;

	sf::Vector2f* last = 0;

	for (int i = 0; i < points.size(); i++)
	{
		sf::Vector2f* cur = &points[i];

		if (last != 0)
			len += thor::length(*cur - *last);

		last = cur;
	}

	return len;
}

sf::Vector2f Worm::GetHeadPos() const
{
	return points.back();
}

sf::Vector2f Worm::GetLastHeadPos()
{
	return lastHead;
}

void Worm::OnEat(int score)
{
	Combo++;
	ComboLeft = ComboTime;

	Score += score * Combo;

	MaxLength += score * 4.0f;

	Multiplier += multAdd;
	multLeft = multTime;

	foodSound.setPitch(Multiplier - 0.2f);
	foodSound.play();
}

bool Worm::OnCollision(CollisionInfo info, GameObject* hit)
{
	bool hitFood = false;

	if (hit == this)
	{
		//std::reverse(points.begin(), points.end());
		//Direction = thor::unitVector(points.rbegin()[0] - points.rbegin()[1]);
		//MaxLength -= Thickness * 12.0f;

		//if (MaxLength <= 0.0f)
		//	return true;

		return true;
	}
	else if (dynamic_cast<Map*>(hit))
	{
		if (Combo > 0)
		{
			ComboLeft = std::min(ComboTime, ComboLeft + 0.4f);
		}

		bounceMult += bounceAdd;
		bounceLeft = bounceTime;

		bounceSound.setPitch(bounceMult);
		bounceSound.play();
	}
	else if (dynamic_cast<Food*>(hit))
	{
		hitFood = true;

		if (Combo > 0)
		{
			ComboLeft = std::min(ComboTime, ComboLeft + ComboTime / 2.0f);
		}

		bounceMult += bounceAdd;
		bounceLeft = bounceTime;

		bounceSound.setPitch(bounceMult);
		bounceSound.play();
	}

	if (info.Normal != sf::Vector2f())
	{
		sf::Vector2f norm = info.Normal;
		float dot = thor::dotProduct(Direction, norm);
		//std::cout << dot << std::endl;

		//if (dot < -0.99)
		//	return true;

		if (hitFood)
			dot = dot / 2.2f;

		Direction = Direction - 2.0f * dot * norm;
		Direction = thor::unitVector(Direction);
		if (dot < 0.0f)
		{
			//Speed -= Speed * fabsf(dot);
			//Speed /= 1.0001f;
		}
	}

	return false;
}

void Worm::addPoint(sf::Vector2f point)
{
	if (points.size() >= 2)
	{
		sf::Vector2f last = points.rbegin()[0];
		sf::Vector2f secondLast = points.rbegin()[1];

		if (last == point) return;

		sf::Vector2f toNew = thor::unitVector(point - last);
		sf::Vector2f toLast = thor::unitVector(last - secondLast);

		if (thor::dotProduct(toNew, toLast) > 0.985f || thor::length(last - secondLast) < 5.0f)
		{
			points[points.size() - 1] = point;
		}
		else
		{
			points.push_back(point);
		}
	}
	else
	{
		points.push_back(point);
	}
};

void Worm::trimPoints(std::vector<sf::Vector2f>& p, float length)
{
	float over = length;

	if (over > 0.0f)
	{
		float firstLen = thor::length(p[0] - p[1]);

		if (firstLen == over) // Ideal situation
		{
			p.erase(p.begin());
		}
		else if (firstLen > over) // Too long
		{
			sf::Vector2f toSecond = thor::unitVector(p[1] - p[0]);
			p[0] = p[0] + toSecond * over;
		}
		else if (firstLen < over) // Not long enough
		{
			p.erase(p.begin());
			over -= firstLen;
			trimPoints(p, over);
		}
	}
}

CollisionInfo Worm::Collides(const Circle& circle)
{
	const float collisionStep = 0.5f;

	sf::Vector2f head = GetHeadPos();
	sf::Vector2f lastHead = GetLastHeadPos();
	sf::Vector2f toHead = thor::length(head - lastHead) > 0 ? thor::unitVector(head - lastHead) : sf::Vector2f();
	sf::Vector2f curHead = head;

	CollisionInfo info;

	for (float j = 0; j <= thor::length(head - lastHead); j += collisionStep)
	{
		curHead = head + toHead * j;

		sf::Vector2f sub = curHead - circle.GetPosition();

		bool test = thor::length(sub) < Thickness / 2.0f + circle.GetRadius();

		if (test)
		{
			info.Collides = true;
			info.Distance = thor::length(sub) - Thickness / 2.0f - circle.GetRadius();
			info.Normal = thor::length(sub) > 0 ? thor::unitVector(-sub) : sf::Vector2f();

			return info;
		}
	}

	return info;
}

std::vector<sf::Vector2f> Worm::GetCollisionPoints()
{
	const float trimAmount = Thickness * 6.0f;
	auto collisionPoints = points;

	if (GetLength() > trimAmount)
	{
		std::reverse(collisionPoints.begin(), collisionPoints.end());
		trimPoints(collisionPoints, trimAmount);
		std::reverse(collisionPoints.begin(), collisionPoints.end());

		return collisionPoints;
	}

	collisionPoints.clear();
	return collisionPoints;
}

void Worm::Tick(float step, int frame)
{
	Speed += Acceleration;
	if (Speed > MaxSpeed)
		Speed = MaxSpeed;

	if (Direction == sf::Vector2f()) return;

	if (points.size() > 0)
		lastHead = points.back();

	sf::Vector2f next = points.back() + Direction * Speed;

	/*
	bool moveLast = points.size() >= 2;

	if (moveLast)
	{
		sf::Vector2f curDir = thor::unitVector(next - points.back());
		sf::Vector2f lastDir = thor::unitVector(points.back() - points.rbegin()[1]);

		float epsilon = 0.01f;
		float dot = thor::dotProduct(curDir, lastDir);
		moveLast = 1.0f - dot < epsilon && 1.0 - dot > -epsilon;
	}

	if (moveLast) // Same direction?
	{
		sf::Vector2f& ref = points.back();
		ref.x = next.x;
		ref.y = next.y;
	}
	else
	{
		addPoint(next);
		//points.push_back(next);
	}*/

	addPoint(next);

	float over = GetLength() - MaxLength;
	if (over > 0.0f)
		trimPoints(points, over);

	if (Multiplier > 1.0f)
	{
		multLeft -= step;
		if (multLeft <= 0.0f)
		{
			multLeft = 0.0f;
			Multiplier = 1.0f;
		}
	}

	if (bounceMult > 1.0f)
	{
		bounceLeft -= step;
		if (bounceLeft <= 0.0f)
		{
			bounceLeft = 0.0f;
			bounceMult = 1.0f;
		}
	}

	if (Combo > 0)
	{
		ComboLeft -= step;
		if (ComboLeft <= 0.0f)
		{
			ComboLeft = 0.0f;
			Combo = 0;
		}
	}
};

void Worm::Draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	sf::Vector2f* last = 0;

	for (int i = 0; i < points.size(); i++)
	{
		sf::Vector2f* cur = &points[i];

		if (last != 0)
		{
			bool drawEnd = false;

			if (i == 1)
				drawEnd = true;

			if (*last - *cur == sf::Vector2f()) continue;

			drawPoints(target, *last, *cur, drawEnd);
		}

		last = cur;
	}
};

void Worm::drawPoints(sf::RenderTarget& target, sf::Vector2f& p1, sf::Vector2f& p2, bool drawP1End)
{
	sf::Vector2f dir = thor::unitVector(p1 - p2);

	sf::Vector2f left = dir;
	thor::rotate(left, -90.0f);

	sf::Vector2f right = dir;
	thor::rotate(right, 90.0f);

	float halfThick = Thickness / 2.0f;

	sf::VertexArray va(sf::PrimitiveType::Quads, 4);
	va[0] = sf::Vertex(p1 + left * halfThick, Color);
	va[1] = sf::Vertex(p2 + left * halfThick, Color);
	va[2] = sf::Vertex(p2 + right * halfThick, Color);
	va[3] = sf::Vertex(p1 + right * halfThick, Color);
	target.draw(va);

	sf::CircleShape circle(halfThick, halfThick * 1.2f);
	circle.setOrigin(halfThick, halfThick);
	circle.setFillColor(Color);

	if (drawP1End)
	{
		circle.setPosition(p1);
		target.draw(circle);
	}

	circle.setPosition(p2);
	target.draw(circle);
};
