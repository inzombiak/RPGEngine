#include "InventoryItem.h"

InventoryItem::InventoryItem(const sf::Texture newSpr, sf::Text newText) : m_text(newText)
{
	m_image = newSpr;
	m_sprite.setTexture(m_image);
}

void InventoryItem::Draw(sf::RenderWindow& rw)
{
	rw.draw(m_sprite);
	rw.draw(m_text);
}


void InventoryItem::Draw(sf::RenderWindow& rw) const
{
	rw.draw(m_sprite);
	rw.draw(m_text);
}

void InventoryItem::SetPosition(sf::Vector2f& newPos)
{
	m_sprite.setPosition(newPos);
	m_text.setPosition(sf::Vector2f(newPos.x + m_sprite.getGlobalBounds().width + m_spriteTextSpacingX, newPos.y));
}

sf::Text& InventoryItem::GetText()
{
	return m_text;
}

const sf::Text& InventoryItem::GetText() const
{
	return m_text;
}