#pragma once

#include <unordered_map>
#include <boost/functional/hash.hpp>

#include "GameObject.hpp"

class ElementGrid : public GameObject
{
public:
	ElementGrid(sf::Vector2i size, sf::Vector2f tileSize) :
		size(size),
		tileSize(tileSize),
		margin(16.0f),
		Selection(sf::Vector2i(0, 0)),
		DrawSelection(true)
	{  };
	virtual ~ElementGrid(void) {  };
	sf::Vector2f GetPosition() { return position; };
	virtual void SetPosition(sf::Vector2f pos) { position = pos; }
	virtual void Draw(sf::RenderTarget& target, sf::RenderStates& states) = 0;
	sf::Vector2i Selection;
	bool DrawSelection;
	bool MoveSelection(sf::Vector2i move)
	{
		sf::Vector2i sign = sf::Vector2i(
			(move.x > 0) - (move.x < 0), 
			(move.y > 0) - (move.y < 0));

		sf::Vector2i newSel = Selection;

		bool ret = false;

		do
		{
			if (sign.x == 0) break;

			newSel += sf::Vector2i(sign.x, 0.0f);

			if (newSel.x >= size.x || newSel.x < 0)
			{
				break;
			}
			else if (validSelection(newSel))
			{
				Selection = newSel;
				ret = true;
				break;
			}
			
		} while (true);

		do
		{
			if (sign.y == 0) break;

			newSel += sf::Vector2i(0.0f, sign.y);

			if (newSel.y >= size.y || newSel.y < 0)
			{
				break;
			}
			else if (validSelection(newSel))
			{
				Selection = newSel;
				ret = true;
				break;
			}
			
			
		} while (true);

		return ret;

		/*
		for (int y = Selection.y + move.y; y < size.y && y > -1; y += sign.y)
		{
			for (int x = Selection.x + move.x; x < size.x && x > -1; x += sign.x)
			{
				sf::Vector2i curSel = sf::Vector2i(x, y);

				if (!validSelection(curSel))
				{
					if (sign.x == 0)
						break;

					continue;
				}

				Selection = curSel;
				return true;
			}
		}

		return false;
		*/
	};
	void SetAction(sf::Vector2i index, std::function<void()> act)
	{
		actions[index] = act;
	};
	void RunSelection()
	{
		if (&actions && actions.size() > 0 && actions[Selection] != nullptr)
			actions[Selection]();
	};
protected:
	virtual bool validSelection(sf::Vector2i sel) = 0;
	sf::Vector2i size;
	sf::Vector2f position;
	sf::Vector2f tileSize;
	float margin;
private:
	struct vectorHasher
	{
		std::size_t operator()(const sf::Vector2i key) const
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, key.x);
			boost::hash_combine(seed, key.y);
			return seed;
		}
	};

	std::unordered_map<sf::Vector2i, std::function<void()>, vectorHasher> actions;
};

