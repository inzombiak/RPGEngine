#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include "ComponentList.h"
#include "SFML/Graphics.hpp"
#include "UIMain.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	window.resetGLStates();
	UIMain m_UI;

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			m_UI.HandleEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (clock.getElapsedTime().asMicroseconds() >= 5000) 
		{
			m_UI.Update(clock.restart().asSeconds());
		}
		window.clear();
		m_UI.Draw(window);
		window.display();
	}
	
	return 0;
}