#include "Map.hpp"

Map::Map(IResponsible* respo, int level) : 
	Area(sf::FloatRect()),
	responsible(respo),
	Inverse(true),
	simuFood(10),
	MaxLayers(0),
	ObstacleColor(sf::Color::White),
	BackgroundColor(sf::Color::Black),
	FoodColor(sf::Color::Green)
{
	createTemplates();

	if (level == -1)
		level = thor::random(0, 2);

	loadTemplate(level);
}

Map::~Map()
{
	GameObject::~GameObject();

	for (int i = obstacles.size() -1 ; i > -1; i--)
	{
		//obstacles.erase();
		responsible->RemoveObject(obstacles[i]);
	}

	obstacles.clear();

	for (int i = Foods.size() - 1; i > -1; i--)
	{
		//obstacles.erase(std::find(obstacles.begin(), obstacles.end(), obstacles[i]));
		responsible->RemoveObject(Foods[i]);
	}
	
	Foods.clear();
}

void Map::loadTemplate(int index)
{
	loadedTemplate = index;

	for (int i = obstacles.size() -1 ; i > -1; i--)
	{
		responsible->RemoveObject(obstacles[i]);
	}
	obstacles.clear();

	for (int i = Foods.size() - 1; i > -1; i--)
	{
		responsible->RemoveObject(Foods[i]);
	}
	Foods.clear();

	for (int i = 0; i < templates[index].size(); i++)
	{
		AddCircleObstacle(templates[index][i]);
	}

	RefreshFoods();
}

void Map::createTemplates()
{
	{
		MapTemplate map = MapTemplate();

		const float cornerDistance = 1400.0f;
		const float ballSize = 400.0f;
		const float tunnelWidth = 60.0f;

		map.push_back(new CircleObstacle(sf::Vector2f(0, 0), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(cornerDistance, 0), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(cornerDistance, cornerDistance), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(0, cornerDistance), ballSize, true));

		map.push_back(new CircleObstacle(sf::Vector2f(cornerDistance / 2.0f, 0), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(cornerDistance / 2.0f, 0), ballSize - tunnelWidth, false));

		map.push_back(new CircleObstacle(sf::Vector2f(cornerDistance, cornerDistance / 2.0f), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(cornerDistance, cornerDistance / 2.0f), ballSize - tunnelWidth, false));

		map.push_back(new CircleObstacle(sf::Vector2f(cornerDistance / 2.0f, cornerDistance), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(cornerDistance / 2.0f, cornerDistance), ballSize - tunnelWidth, false));

		map.push_back(new CircleObstacle(sf::Vector2f(0, cornerDistance / 2.0f), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(0, cornerDistance / 2.0f), ballSize - tunnelWidth, false));

		templates.push_back(map);
	}

	{
		MapTemplate map = MapTemplate();

		const float ballSize = 1000.0f;
		const float tunnelWidth = 300.0f;

		map.push_back(new CircleObstacle(sf::Vector2f(ballSize - tunnelWidth / 2.0f, 0), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(ballSize - tunnelWidth / 2.0f, 0), ballSize - tunnelWidth, false));

		templates.push_back(map);
	}

	{
		MapTemplate map = MapTemplate();

		const float ballSize = 400.0f;
		const float tunnelWidth = 200.0f;

		map.push_back(new CircleObstacle(sf::Vector2f(ballSize - tunnelWidth / 2.0f, 0), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(ballSize - tunnelWidth / 2.0f, 0), ballSize - tunnelWidth, false));
		map.push_back(new CircleObstacle(sf::Vector2f(ballSize - tunnelWidth / 2.0f + ballSize * 2.0f - tunnelWidth, 0), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(ballSize - tunnelWidth / 2.0f + ballSize * 2.0f - tunnelWidth, 0), ballSize - tunnelWidth, false));

		map.push_back(new CircleObstacle(sf::Vector2f(ballSize - tunnelWidth / 2.0f + (ballSize * 2.0f - tunnelWidth) / 2.0f, -(ballSize * 2.0f - tunnelWidth * sqrtf(1.9f))), ballSize, true));
		map.push_back(new CircleObstacle(sf::Vector2f(ballSize - tunnelWidth / 2.0f + (ballSize * 2.0f - tunnelWidth) / 2.0f, -(ballSize * 2.0f - tunnelWidth * sqrtf(1.9f))), ballSize - tunnelWidth, false));

		templates.push_back(map);
	}
}

Map* Map::CloneSettingsOnly()
{
	Map* newMap = new Map(responsible, loadedTemplate);
	newMap->SetObstacleColor(ObstacleColor);
	newMap->SetBackgroundColor(BackgroundColor);
	newMap->SetFoodColor(FoodColor);
	newMap->SetFoodLayerColor(FoodLayerColor);

	return newMap;
}

void Map::SetFoodColor(sf::Color color)
{
	FoodColor = color;

	for (int i = 0; i < Foods.size(); i++)
	{
		Foods[i]->Color = color;
	}
}

void Map::SetFoodLayerColor(sf::Color color)
{
	FoodLayerColor = color;

	for (int i = 0; i < Foods.size(); i++)
	{
		Foods[i]->LayerColor = color;
	}
}

void Map::SetObstacleColor(sf::Color color)
{
	ObstacleColor = color;

	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i]->Color = color;
	}
}

void Map::SetBackgroundColor(sf::Color color)
{
	BackgroundColor = color;

	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i]->BackgroundColor = color;
	}
}

void Map::OnEat()
{
	foodEaten++;

	RefreshFoods();
}

void Map::RefreshFoods()
{
	while (Foods.size() < simuFood)
	{
		AddRandomFood();
	}
}

void Map::AddRandomFood()
{
	const float margin = 100.0f;
	sf::FloatRect rect = Area;

	Food* food;
	do
	{
		food = new Food(sf::Vector2f(
			thor::random(rect.left, rect.left + rect.width), 
			thor::random(rect.top, rect.top + rect.height)), thor::random(0, MaxLayers));

		if (Collides(*food).Collides)
			continue;

		break;

	} while (true);

	food->Color = FoodColor;
	food->LayerColor = FoodLayerColor;

	Foods.push_back(food);
	responsible->AddObject(food);
}

// TODO: FIX! This is a big mess, bunch of edge cases not accounted for
CollisionInfo Map::Collides(const Circle& circle)
{
	for (int i = 0; i < obstacles.size(); i++)
	{
		CollisionInfo info = obstacles[i]->Collides(circle);

		if (!info.Inverse && info.Collides)
		{
			return info;
		}
	}

	std::vector<CollisionInfo> collides;

	for (int i = 0; i < obstacles.size(); i++)
	{
		CollisionInfo info = obstacles[i]->Collides(circle);

		if (!info.Inverse) continue;

		if (Inverse)
		{
			if (!info.Collides)
			{
				return info;
			}
			else
			{
				collides.push_back(info);
			}
		}
		else
		{
			if (info.Collides)
			{
				return info;
			}
		}
	}

	if (Inverse)
	{
		if (collides.size() <= 1)
			return collides[0];

		bool allCollide = true;
		for (int i = 0; i < collides.size(); i++)
		{
			if (!collides[i].Collides)
				allCollide = false;
		}

		float thick = 0;

		sf::Vector2f normal = sf::Vector2f();

		CollisionInfo lowest;
		lowest.Collides = true;
		for (int i = 0; i < collides.size(); i++)
		{
			thick += circle.GetRadius() / 1.4f - collides[i].Distance;

			normal += collides[i].Normal;

			if (i == 0 || collides[i].Distance < lowest.Distance)
			{
				//lowest.Inverse = collides[i].Inverse;
				//lowest.Distance = collides[i].Distance;
				lowest = collides[i];
			}
		}

		//lowest.Normal = lowest.Normal * (1.0f / collides.size());

		if (thick > 0.0f)
			return CollisionInfo();
		else
		{
			if (fabsf(thick) < 0.5f && thor::length(normal) != 0)
			{
				std::cout << thick << std::endl;
				
				normal = normal * (1.0f / collides.size());
				lowest.Normal = thor::unitVector(normal);
				//thor::rotate(lowest.Normal, -90.0f);
			}
			return lowest;
		}
	}

	return CollisionInfo();
}

CircleObstacle* Map::AddCircleObstacle(CircleObstacle* obs)
{
	obs->Color = ObstacleColor;
	obs->BackgroundColor = BackgroundColor;

	obstacles.push_back(obs);
	responsible->AddObject(obs, -1);

	sf::FloatRect rect = obs->GetRect();

	if (rect.left < Area.left)
	{
		Area.width += Area.left - rect.left;
		Area.left = rect.left;
	}

	if (rect.top < Area.top)
	{
		Area.height += Area.top - rect.top;
		Area.top = rect.top;
	}

	//Area.left = std::min(Area.left, rect.left);
	//Area.top = std::min(Area.top, rect.top);

	if (rect.left + rect.width > Area.left + Area.width)
		Area.width = rect.left + rect.width - Area.left;

	if (rect.top + rect.height > Area.top + Area.height)
		Area.height = rect.top + rect.height - Area.top;

	if (rect.left < Area.left)
	{
		Area.width += Area.left - rect.left;
		Area.left = rect.left;
	}

	if (rect.top < Area.top)
	{
		Area.height += Area.top - rect.top;
		Area.top = rect.top;
	}

	return obs;
}

CircleObstacle* Map::AddCircleObstacle(sf::Vector2f pos, float radius, bool inverse)
{
	CircleObstacle* obs = new CircleObstacle(pos, radius, inverse);

	return AddCircleObstacle(obs);
}
