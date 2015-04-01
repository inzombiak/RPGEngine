#include "ItemOptionsMenu.h"


ItemOptionsMenu::ItemOptionsMenu(sf::Font font)
{
	m_font = font;
	use.setFont(m_font);
	drop.setFont(m_font);
	deleteText.setFont(m_font);
	use.setCharacterSize(14);
	use.setString("Use");
	drop.setString("Drop");
	drop.setCharacterSize(14);
	deleteText.setString("Delete");
	deleteText.setCharacterSize(14);
	sf::Vector2f rectSize(deleteText.getLocalBounds().width + 8, deleteText.getLocalBounds().height + 4);
	useRect.setSize(rectSize);
	useRect.setFillColor(sf::Color::Black);
	dropRect.setSize(rectSize);
	dropRect.setFillColor(sf::Color::Black);
	deleteRect.setSize(rectSize);
	deleteRect.setFillColor(sf::Color::Black);
	SetPosition(sf::Vector2f(0, 0));
}

void ItemOptionsMenu::Draw(sf::RenderWindow& rw)
{
	if (!isOpen)
		return;
	rw.draw(useRect);
	rw.draw(use);
	rw.draw(dropRect);
	rw.draw(drop);
	rw.draw(deleteRect);
	rw.draw(deleteText);
}

int ItemOptionsMenu::HandleInput(sf::Event, sf::Vector2f mousePos)
{
	if (useRect.getGlobalBounds().contains(mousePos))
		return 1;
	else if (dropRect.getGlobalBounds().contains(mousePos))
		return 2;
	else if (deleteRect.getGlobalBounds().contains(mousePos))
		return 3;
	else
		return 0;
}