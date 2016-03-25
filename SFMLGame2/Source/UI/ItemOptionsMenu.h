#include "..\Utilities\GameDefs.h"
#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"

class ItemOptionsMenu
{
public:
	ItemOptionsMenu(sf::Font font);
	void SetPosition(sf::Vector2f pos)
	{
		m_pos = pos;
		useRect.setPosition(pos);
		dropRect.setPosition(sf::Vector2f(pos.x, pos.y + dropRect.getSize().y));
		deleteRect.setPosition(sf::Vector2f(sf::Vector2f(pos.x, pos.y + 2 * dropRect.getSize().y)));
		use.setPosition(sf::Vector2f(pos.x + 4, pos.y + 2));
		drop.setPosition(sf::Vector2f(pos.x + 4, use.getPosition().y + useRect.getSize().y));
		deleteText.setPosition(sf::Vector2f(sf::Vector2f(pos.x + 4, use.getPosition().y + 2 * useRect.getSize().y)));
	}
	void SetOpen(bool open)
	{
		isOpen = open;
	}
	bool IsOpen()
	{
		return isOpen;
	}
	void AttachItem(ItemID item)
	{
		attachedItem = item;
	}
	ItemID GetAttachedItem()
	{
		return attachedItem;
	}
	void Draw(sf::RenderWindow& rw);
	int HandleInput(sf::Event, sf::Vector2f mousePos);
private:
	sf::Font m_font;
	sf::Vector2f m_pos;
	sf::Text use;
	sf::Text drop;
	sf::Text deleteText;
	sf::RectangleShape useRect;
	sf::RectangleShape dropRect;
	sf::RectangleShape deleteRect;
	bool isOpen = false;
	ItemID attachedItem;
};