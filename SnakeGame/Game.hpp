#pragma once

#include <SFML/Graphics.hpp>
#include "Thor\Vectors.hpp"
//#include <Thor\Resources.hpp>
#include <SFML\Audio.hpp>
#include <sstream>

#include "IResponsible.hpp"
#include "PlayerWorm.hpp"
#include "Food.hpp"
#include "Map.hpp"
#include "TextGrid.hpp"
#include "ShadowTextRenderer.hpp"
#include "ColorGrid.hpp"
#include "Settings.hpp"
#include "Utilities.hpp"

class Game : public GameObject, public IResponsible
{
public:
	Game();
	~Game();
	void Run();
	virtual void Tick(float step, int frame);
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates& states);
private:
	sf::RenderWindow *rw;
	const float step;
	float avgDt;
	float acc;
	float t;
	int frame;
	sf::Clock clock;

	void handleResize(); // Resize stuff according to the window if needed

	void gotoMainMenu();
	TextGrid* mainGrid;
	sf::Text* titleText;
	
	void gotoGameSetup();
	TextGrid* backGrid;
	TextGrid* levelGrid;
	ColorGrid* colorGrid;
	ElementGrid* curSetupGrid;
	int nextLevel;

	void gotoPauseMenu();
	TextGrid* pauseGrid;
	sf::RectangleShape* pauseBg;

	void gotoDeath();
	sf::Text* retryText;

	void handleKeyDown(sf::Keyboard::Key key);
	void handleKeyUp(sf::Keyboard::Key key);

	void gotoGame(int level = -1);
	PlayerWorm* player;
	void replacePlayer();
	Map* map;

	sf::Font* font;
	sf::Text* comboText;
	sf::Text* scoreText;
	sf::Shader* gaussianShader;

	sf::RectangleShape* comboRect;
	float comboRectWidth;

	const float targetFps;

	ShadowTextRenderer* textRenderer;

	void setMapColors(sf::Color base);

	
	//std::map<sf::Vector2u, sf::RenderTarget*> shadowCache;

	//thor::ResourceKey<sf::Image> ship1Key;
	//thor::ResourceCache<sf::Image> imageCache;
};

