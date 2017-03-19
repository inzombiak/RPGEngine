#pragma once
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include <list>

class MainMenu
{
public:
	MainMenu()
	{
		m_font.loadFromFile("data/arial.ttf");
	}
	enum MenuResult { Nothing, Exit, Play };

	struct MenuItem
	{
	public:
		void create(int, int, int, int, sf::Text, MenuResult);
		const sf::Sprite getSprite() const;
		const sf::Text	getText() const;
		sf::Rect<float> getBounds();
		MenuResult getAction();
	private:
		MenuResult action;
		sf::Texture texture;
		sf::Sprite sprite;
		sf::Text m_text;
	};

	MenuResult Show(sf::RenderWindow& window);

private:
	sf::Font m_font;

	MenuResult GetMenuResponse(sf::RenderWindow& window);
	MenuResult HandleClick(int x, int y);
	void DrawAll(sf::RenderWindow& window);
	std::list<MenuItem> m_menuItems;
	int m_buttonHeight = 40;
	int m_buttonWidth = 200;
	int m_buttonDistance = 30;
};