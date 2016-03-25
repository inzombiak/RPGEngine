#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"


class InventoryItem
{
public:
	InventoryItem(const sf::Texture , sf::Text);
	void Draw(sf::RenderWindow&);
	void Draw(sf::RenderWindow&) const;
	void SetPosition(sf::Vector2f&);

	sf::Text& GetText();
	const sf::Text& GetText() const;

private:
	const int m_spriteTextSpacingX = 6;
	sf::Vector2f m_position;
	sf::Texture m_image;
	sf::Sprite m_sprite;
	sf::Text m_text;
};

#endif