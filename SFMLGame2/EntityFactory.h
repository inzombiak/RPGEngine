#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "tinyxml2.h"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "custom_defininitions.h"

#include <string>

using tinyxml2::XMLElement;
class EntityFactory
{
public:
	bool CreateEntity(XMLElement* currEntityNode, StrongEntityPtr entity);
	//Entity CreateEntityByCopy(XMLElement* currEntityNode);
	bool CreateTileFromTmx(sf::Vector2f position, sf::Texture& texture, sf::IntRect textureRect,int depth, StrongEntityPtr entity);//For use with Tiled map files to creates tiles
	//Entity CreateTileFromTmxByCopy(sf::Vector2f position, sf::Texture& texture, sf::IntRect textureRect);
	//For use with Tiles map files to create collision boxes
	bool CreateCollisionEntity(sf::Vector2f position, std::vector<sf::Vector2f> points, string type, int depth, StrongEntityPtr newEntity);
	//Entity CreateCollisionEntityByCopy(sf::Vector2f position, sf::Vector2f dimensions);
	static EntityFactory* GetInstance()
	{
		static EntityFactory instance;
		return &instance;
	}
	EntityNameID GetEntityNameIDFromName(const std::string& name)
	{
		return reinterpret_cast<EntityNameID>(HashedString::hash_name(name.c_str()));
	}
private:
	EntityFactory();
	EntityFactory(const EntityFactory &) { }
	EntityFactory &operator=(const EntityFactory &) { return *this; }
	static int GetNextEntityID()
	{
		m_lastEntityID++;
		return m_lastEntityID;
	}
	StrongComponentPtr CreateComponent(XMLElement* currComponentNode);
	
	static int m_lastEntityID;
};

#endif