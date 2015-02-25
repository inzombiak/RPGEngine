#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "custom_defininitions.h"

using std::vector;

class EntityRenderer
{
public:
	EntityRenderer(sf::RenderWindow& rw) : m_renderWindow(rw)
	{

	};
	void Draw(WeakEntityPtr entity);

private:
	sf::RenderWindow& m_renderWindow;
};

#endif