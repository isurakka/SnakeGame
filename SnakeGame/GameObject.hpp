#pragma once

#include <SFML\Graphics.hpp>

enum class GameStates
{
	Game = 1,
	Death = 2,
	MainMenu = 4,
	GameSetup = 8,
	PauseMenu = 16
};

inline GameStates operator|(GameStates a, GameStates b)
{ return static_cast<GameStates>(static_cast<int>(a) | static_cast<int>(b)); };

inline GameStates operator&(GameStates a, GameStates b)
{ return static_cast<GameStates>(static_cast<int>(a) & static_cast<int>(b)); };

inline GameStates operator^(GameStates a, GameStates b)
{ return static_cast<GameStates>(static_cast<int>(a) ^ static_cast<int>(b)); };

class GameObject
{
public:
	GameObject() : Enabled(true), GameState(GameStates::Game) {  }
	virtual ~GameObject() { };
	bool Enabled;
	GameStates GameState;
	virtual void Update(float dt) { };
	virtual void Tick(float step, int frame) { };
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates& states) { };
};