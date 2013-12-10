#pragma once

#include "boost\multi_array.hpp"
#include "Thor\Math.hpp"

#include "ElementGrid.hpp"
#include "Utilities.hpp"

typedef boost::multi_array<sf::RectangleShape*, 2> RectMap;
typedef RectMap::index index;

class ColorGrid : public ElementGrid
{
public:
	ColorGrid(sf::Vector2i size, sf::Vector2f tileSize);
	~ColorGrid(void);
	virtual void SetPosition(sf::Vector2f pos);
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates& states);
	sf::Color GetSelectionColor();
	static sf::Color HSVToRGB(float hue, float saturation, float value);
	static float RGBToHSV(sf::Color color);
	static sf::Color GeneratePrettyColor(); // Generate pretty color using golden ratio
	static std::vector<sf::Color> GeneratePrettyColors(int count);
protected:
	virtual bool validSelection(sf::Vector2i sel);
private:
	RectMap rects;
};

