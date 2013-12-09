#pragma once

#include <string>
#include "boost\multi_array.hpp"

#include "ElementGrid.hpp"
#include "ShadowTextRenderer.hpp"
#include "Settings.hpp"

typedef boost::multi_array<sf::Text*, 2> TextMap;
typedef TextMap::index index;

class TextGrid : public ElementGrid
{
public:
	TextGrid(sf::Vector2i size, sf::Vector2f tileSize, sf::Shader* shader);
	virtual ~TextGrid(void);
	virtual void SetPosition(sf::Vector2f pos);
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates& states);
	void SetText(sf::Vector2i index, std::string text);
protected:
	virtual bool validSelection(sf::Vector2i sel);
private:
	TextMap texts;
	sf::Font* font;
	ShadowTextRenderer* textRenderer;
};

