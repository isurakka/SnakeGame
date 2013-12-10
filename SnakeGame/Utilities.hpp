#pragma once

#include <SFML/Graphics.hpp>

namespace Utilities
{
	static sf::View SetDefaultView(sf::RenderTarget& target) // Needed for window resizing. Also why doesn't SFML do this by default?
	{
		sf::View oldView = target.getView();
		sf::View newView = target.getDefaultView();
		newView.setSize(target.getSize().x, target.getSize().y);
		sf::Vector2f topLeft = target.mapPixelToCoords(sf::Vector2i(), newView);
		newView.setCenter(newView.getCenter() - topLeft);
		target.setView(newView);

		return oldView;
	}
}
