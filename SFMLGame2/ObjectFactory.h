#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "VisibleGameObject.h"
#include "Tile.h"
#include "Player.h"
#include "RenderComponent.h"
#include <memory>
#include <string>

using std::string;

class ObjectFactory
{
public:
	static VisibleGameObject* GenerateObject(string name, int[4]);
	static VisibleGameObject* GenerateCollisionBox(int[4]);
private:
	ObjectFactory();
	ObjectFactory* m_objectFactory;
};

#endif