#pragma once

#include <SFML/Graphics.hpp>

struct vectorHasher
{
	std::size_t operator()(const sf::Vector2u key) const
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, key.x);
		boost::hash_combine(seed, key.y);
		return seed;
	}
};

class ShadowTextRenderer
{
public:
	ShadowTextRenderer(sf::Shader* gaussianShader)
	{
		this->gaussianShader = gaussianShader;
	}

	~ShadowTextRenderer(void)
	{
		
	}

	void ShadowTextRenderer::DrawDropShadowText(sf::RenderTarget& target, sf::Text& text, bool highQuality = true)
	{
		sf::RenderStates states = sf::RenderStates();
		states.blendMode = sf::BlendMode::BlendAlpha;

		if (gaussianShader && highQuality)
		{
			sf::FloatRect bounds = text.getGlobalBounds();
			sf::Vector2u size = sf::Vector2u(bounds.width * 2.0f, bounds.height * 4.0f);

			auto ptr = findFromTextureCache(size);
			sf::RenderTexture* base;
			if (ptr != 0)
				base = ptr;
			else
			{
				base = new sf::RenderTexture();
				base->create(size.x, size.y);
				base->setSmooth(true);
			}

			sf::View view = base->getView();
			view.setCenter(text.getPosition().x + bounds.width / 2.0f, text.getPosition().y + bounds.height / 2.0f);
			base->setView(view);

			base->clear(sf::Color::Transparent);

			sf::Color oldColor = text.getColor();
			text.setColor(sf::Color::Black);
			base->draw(text, states);
			text.setColor(oldColor);

			base->display();

			ptr = findFromTextureCache(size);
			sf::RenderTexture* shadow;
			if (ptr != 0)
				shadow = ptr;
			else
			{
				shadow = new sf::RenderTexture();
				shadow->create(size.x, size.y);
				shadow->setSmooth(true);
			}

			view = shadow->getView();
			view.setCenter(shadow->getSize().x / 2.0f, shadow->getSize().y / 2.0f);
			shadow->setView(view);

			shadow->clear(sf::Color::Transparent);

			gaussianShader->setParameter("texture", base->getTexture());
			gaussianShader->setParameter("resolution", bounds.width);
			gaussianShader->setParameter("blurScale", 0.4f);
			gaussianShader->setParameter("blurStrength", 0.5f);
			gaussianShader->setParameter("blurAmount", 6.0f);
			gaussianShader->setParameter("dir", sf::Vector2f(1.0f, 0.0f));

			states.shader = gaussianShader;

			shadow->draw(sf::Sprite(base->getTexture()), states);
			shadow->display();

			addToTextureCache(base);

			sf::Texture shadowTex = shadow->getTexture();
			shadow->clear(sf::Color::Transparent);

			gaussianShader->setParameter("texture", shadowTex);
			gaussianShader->setParameter("resolution", bounds.height);
			gaussianShader->setParameter("dir", sf::Vector2f(0.0f, 1.0f));

			states.shader = gaussianShader;

			shadow->draw(sf::Sprite(shadowTex), states);
			shadow->display();

			states.shader = 0;

			sf::Sprite shadowSpr = sf::Sprite(shadow->getTexture());
			shadowSpr.setPosition(text.getPosition() - sf::Vector2f(shadow->getSize().x / 2.0f , shadow->getSize().y / 2.0f) + sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f));
			target.draw(shadowSpr, states);

			addToTextureCache(shadow);

			//sf::Sprite baseSpr = sf::Sprite(base.getTexture());
			//baseSpr.setPosition(text.getPosition() + (text.getPosition() - view.getCenter()));
		}
		else
		{
			sf::Vector2f oldPos = text.getPosition();
			text.setPosition(oldPos - sf::Vector2f(2.0f, 2.0f));

			sf::Color oldColor = text.getColor();
			text.setColor(sf::Color::Black);

			target.draw(text, states);

			text.setPosition(oldPos);
			text.setColor(oldColor);
		}

		target.draw(text, states);
	}

private:
	sf::Shader* gaussianShader;

	std::unordered_multimap<sf::Vector2u, sf::RenderTexture*, vectorHasher> shadowCache;

	sf::RenderTexture* ShadowTextRenderer::findFromTextureCache(sf::Vector2u size)
	{
		auto it = shadowCache.find(size);

		if (it != shadowCache.end())
		{
			auto ptr = it->second;
			shadowCache.erase(it);
			return ptr;
		}

		return 0;
	}

	void ShadowTextRenderer::addToTextureCache(sf::RenderTexture* tex)
	{
		shadowCache.insert(std::pair<sf::Vector2u, sf::RenderTexture*>(tex->getSize(), tex));
	}
};