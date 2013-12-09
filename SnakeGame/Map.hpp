#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>

#include "IResponsible.hpp"
#include "GameObject.hpp"
#include "CircleObstacle.hpp"

#include "Worm.hpp"
#include "Food.hpp"

typedef std::vector<CircleObstacle*> MapTemplate;

class Map : public GameObject, public ICollider
{
public:
	Map(IResponsible* respo, int level = -1);
	virtual ~Map();
	Map* CloneSettingsOnly();
	CircleObstacle* AddCircleObstacle(sf::Vector2f pos, float radius, bool inverse = true);
	CircleObstacle* AddCircleObstacle(CircleObstacle* obs);
	virtual CollisionInfo Collides(const Circle& circle);
	sf::FloatRect Area;
	float Inverse;
	std::vector<Food*> Foods;
	void RefreshFoods();
	void AddRandomFood();
	void OnEat();
	int MaxLayers;
	
	void SetObstacleColor(sf::Color color);
	sf::Color ObstacleColor;
	void SetBackgroundColor(sf::Color color);
	sf::Color BackgroundColor;
	void SetFoodColor(sf::Color color);
	sf::Color FoodColor;
	void SetFoodLayerColor(sf::Color color);
	sf::Color FoodLayerColor;
private:
	int loadedTemplate;
	int foodEaten;
	int simuFood;
	
	std::vector<CircleObstacle*> obstacles;

	void createTemplates();
	std::vector<MapTemplate> templates;

	void loadTemplate(int index);

	IResponsible* responsible;
};

