#include "ColorGrid.hpp"


ColorGrid::ColorGrid(sf::Vector2i size, sf::Vector2f tileSize) :
	ElementGrid(size, tileSize),
	rects(RectMap(boost::extents[size.y][size.x]))
{
	margin = 6.0f;

	auto colors = GeneratePrettyColors(size.x * size.y);
	int curColor = 0;

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			sf::RectangleShape* rect = new sf::RectangleShape(tileSize);
			rect->setFillColor(colors[curColor++]);
			rect->setOutlineThickness(-1.0f);
			rect->setOutlineColor(sf::Color::Black);
			rects[y][x] = rect;
		}
	}
}

ColorGrid::~ColorGrid(void)
{

}

sf::Color ColorGrid::GetSelectionColor()
{
	return rects[Selection.y][Selection.x]->getFillColor();
};

void ColorGrid::SetPosition(sf::Vector2f pos)
{
	ElementGrid::SetPosition(pos);

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			sf::RectangleShape* rect = rects[y][x];
			sf::Vector2f newPos = position + sf::Vector2f(x * tileSize.x, y * tileSize.y) + sf::Vector2f(margin * x, margin * y);
			rect->setPosition(newPos);
		}
	}
}

void ColorGrid::Draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	sf::View oldView = Utilities::SetDefaultView(target);

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			sf::RectangleShape* rect = rects[y][x];

			if (!validSelection(sf::Vector2i(x, y))) continue;

			if (DrawSelection && Selection == sf::Vector2i(x, y))
			{
				sf::RectangleShape selRect(tileSize + sf::Vector2f(margin, margin));
				selRect.setPosition(rect->getPosition() - sf::Vector2f(margin / 2.0f, margin / 2.0f));
				selRect.setFillColor(sf::Color(0, 0, 0, 200));
				target.draw(selRect);
			}

			target.draw(*rect, states);
		}
	}

	target.setView(oldView);
}

bool ColorGrid::validSelection(sf::Vector2i sel)
{
	return true;
}

sf::Color ColorGrid::GeneratePrettyColor()
{
	float offset = thor::random(0.0f, 1.0f);
	
	return HSVToRGB(fmod(offset + 0.618033988749895f, 1.0f) * 360.0f, 1.0f, 1.0f);
}

std::vector<sf::Color> ColorGrid::GeneratePrettyColors(int count)
{
	float offset = thor::random(0.0f, 1.0f);

	std::vector<sf::Color> colors;
	for (int i = 0; i < count; i++)
	{
		colors.push_back(HSVToRGB(fmod(offset + (0.618033988749895f * i), 1.0f) * 360.0f, 1.0f, 1.0f));
	}
	return colors;
}

sf::Color ColorGrid::HSVToRGB(float hue, float saturation, float value)
{
    int hi = (int)floor(hue / 60.0) % 6;
    double f = hue / 60.0 - floor(hue / 60.0);

    value = value * 255.0f;
    int v = value;
    int p = value * (1 - saturation);
    int q = value * (1 - f * saturation);
    int t = value * (1 - (1 - f) * saturation);

    if (hi == 0)
        return sf::Color(v, t, p, 255);
    else if (hi == 1)
        return sf::Color(q, v, p, 255);
    else if (hi == 2)
        return sf::Color(p, v, t, 255);
    else if (hi == 3)
        return sf::Color(p, q, v, 255);
    else if (hi == 4)
        return sf::Color(t, p, v, 255);
    else
        return sf::Color(v, p, q, 255);
}

float ColorGrid::RGBToHSV(sf::Color color)
{
	float r = (float)color.r / 255.0f;
	float g = (float)color.g / 255.0f;
	float b = (float)color.b / 255.0f;

	float min, max, delta;

	min = std::min(r, g);
	min = std::min(min, b);

	max = std::max(r, g);
	max = std::max(max, b);

	float h, s, v;

	v = max;
	delta = max - min;

	if( max != 0 )
		s = delta / max;
	else {
		s = 0;
		h = -1;
		return h;
	}

	if (r == max)
		h = (g - b) / delta;		// between yellow & magenta
	else if (g == max)
		h = 2 + (b - r) / delta;	// between cyan & yellow
	else
		h = 4 + (r - g) / delta;	// between magenta & cyan

	h *= 60;						// degrees
	if (h < 0)
		h += 360;

	return h;
}
