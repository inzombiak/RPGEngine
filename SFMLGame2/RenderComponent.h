#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "SFML\Graphics.hpp"
#include "ObjectComponent.h"



class RenderComponent : public ObjectComponent
{
public:
	const sf::Texture& GetTexture() const;
	void SetTexture(const sf::Texture& texture);

	void SetPosition(const sf::Vector2f position);

	void SetVisible(bool isVisible);
	const bool IsVisible() const;

	const sf::Sprite& GetSprite();
	void SetSprite(sf::Sprite sprite);

	void SetTextureRect(sf::Rect<int> textureBounds);
	sf::Rect<int> GetTextureRect() const;

private:
	sf::Sprite m_objectSprite;
	sf::Texture m_texture;
	bool m_visible;
};

#endif