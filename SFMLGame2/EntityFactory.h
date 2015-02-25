#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "tinyxml2.h"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "ComponentFactory.h"

using tinyxml2::XMLElement;

class EntityFactory
{
public:
	EntityFactory();

	StrongEntityPtr CreateEntity(XMLElement* currEntityNode);
	StrongEntityPtr CreateTileFromTmx(sf::Vector2f position, sf::Texture& texture, sf::IntRect textureRect); //For use with Tiled map files to creates tiles
	StrongEntityPtr CreateCollisionEntity(sf::Vector2f position, sf::Vector2f dimensions); //For use with Tiles map files to create collision boxes

protected:


private:

	static EntityID GetNextEntityID()
	{
		m_lastEntityID++;
		return m_lastEntityID;
	}

	StrongComponentPtr CreateComponent(XMLElement* currComponentNode);

	static EntityID m_lastEntityID;

	ComponentFactory m_componentFactory;
};

#endif