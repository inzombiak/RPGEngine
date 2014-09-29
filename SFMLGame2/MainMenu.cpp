#include "MainMenu.h"

MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& rw)
{
	sf::Vector2u windowSize = rw.getSize();

	MenuItem play;
	play.create(windowSize.x / 2 - m_buttonWidth / 2, 0, m_buttonWidth, m_buttonHeight, Play);

	MenuItem exit;
	exit.create(windowSize.x / 2 - m_buttonWidth / 2, m_buttonDistance + m_buttonHeight, m_buttonWidth, m_buttonHeight, Exit);

	m_menuItems.push_back(play);
	m_menuItems.push_back(exit);

	DrawAll(rw);
	return GetMenuResponse(rw);
}


MainMenu::MenuResult MainMenu::HandleClick(int x, int y)
{
	std::list<MenuItem>::iterator it;

	for (it = m_menuItems.begin(); it != m_menuItems.end(); it++)
	{
		sf::Rect<float> menuItemRect = it->getBounds();
		if (menuItemRect.top  + menuItemRect.height > y
			&& menuItemRect.top < y
			&& menuItemRect.left < x
			&& menuItemRect.left + menuItemRect.width > x)
		{
			return it->getAction();
		}
	}
	return Nothing;
}

void MainMenu::DrawAll(sf::RenderWindow& rw)
{
	std::list<MenuItem>::iterator it;

	for (it = m_menuItems.begin(); it != m_menuItems.end(); it++)
	{
		rw.draw(it->getSprite());
	}
	rw.display();
}

MainMenu::MenuResult MainMenu::GetMenuResponse(sf::RenderWindow& window)
{
	sf::Event menuEvent;

	while (true)
	{
		while (window.pollEvent(menuEvent))
		{
			if (menuEvent.type == sf::Event::MouseButtonPressed)
				return HandleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
			if (menuEvent.type == sf::Event::Closed)
				return Exit;
		}

	}
}

void MainMenu::MenuItem::create(int x, int y, int w, int h, MenuResult act)
{
	sf::Texture t;
	bool r = t.create(w, h);
	texture = t;
	sprite.setTexture(texture);
	sprite.setColor(sf::Color(255, 255, 0, 255));
	sprite.setPosition(x, y);
	action = act;
}

sf::Sprite MainMenu::MenuItem::getSprite()
{
	return sprite;
}

sf::Rect<float> MainMenu::MenuItem::getBounds()
{
	return sprite.getGlobalBounds();
}

MainMenu::MenuResult MainMenu::MenuItem::getAction()
{
	return action;
}