#include "TextGrid.hpp"

TextGrid::TextGrid(sf::Vector2i size, sf::Vector2f tileSize, sf::Shader* shader) :
	ElementGrid(size, tileSize),
	texts(TextMap(boost::extents[size.y][size.x]))
{
	font = new sf::Font();
	font->loadFromFile("assets/DejaVuSans.ttf");

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			sf::Text* text = new sf::Text("", *font, tileSize.y);
			texts[y][x] = text;
		}
	}

	textRenderer = new ShadowTextRenderer(shader);
}

TextGrid::~TextGrid(void)
{

}

bool TextGrid::validSelection(sf::Vector2i sel)
{
	return texts[sel.y][sel.x]->getString() != sf::String();
}

void TextGrid::SetPosition(sf::Vector2f pos)
{
	ElementGrid::SetPosition(pos);

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			sf::Text* text = texts[y][x];
			sf::Vector2f newPos = position + sf::Vector2f(x * tileSize.x, y * tileSize.y) + sf::Vector2f(margin * x, margin * y);
			text->setPosition(newPos);
		}
	}
}

void TextGrid::SetText(sf::Vector2i pos, std::string text)
{
	texts[pos.y][pos.x]->setString(text);
	SetPosition(GetPosition());
}

void TextGrid::Draw(sf::RenderTarget& target, sf::RenderStates& states)
{
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());

	//states.blendMode = sf::BlendMode::BlendNone;

	bool high = Settings::Quality <= GraphicsQuality::Medium;

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			sf::Text* text = texts[y][x];

			if (text->getString() == "") continue;
			
			if (DrawSelection && Selection == sf::Vector2i(x, y))
			{
				sf::FloatRect bounds = text->getGlobalBounds();

				const float selAdd = 6.0f;

				sf::RectangleShape rect(sf::Vector2f(tileSize.x, tileSize.y + selAdd * 2.0f));
				rect.setPosition(text->getPosition());
				target.draw(rect);

				/*
				sf::RectangleShape rect(tileSize);
				rect.setPosition(text->getPosition());
				*/

				rect.setFillColor(sf::Color(0, 0, 0, 0));
				target.draw(rect);
			}

			//target.draw(*text, states);
			textRenderer->DrawDropShadowText(target, *text, high);
		}
	}

	target.setView(oldView);
}