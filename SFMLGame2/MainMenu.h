#pragma once
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include <list>

class MainMenu
{
public:
	enum MenuResult { Nothing, Exit, Play };

	struct MenuItem
	{
	public:
		void create(int, int, int, int, MenuResult);
		sf::Sprite getSprite();
		sf::Rect<float> getBounds();
		MenuResult getAction();
	private:
		MenuResult action;
		sf::Texture texture;
		sf::Sprite sprite;
	};

	MenuResult Show(sf::RenderWindow& window);

private:
	MenuResult GetMenuResponse(sf::RenderWindow& window);
	MenuResult HandleClick(int x, int y);
	void DrawAll(sf::RenderWindow& window);
	std::list<MenuItem> m_menuItems;
	int m_buttonHeight = 40;
	int m_buttonWidth = 200;
	int m_buttonDistance = 30;
};