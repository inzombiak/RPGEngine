#include "ObjectFactory.h"

VisibleGameObject* ObjectFactory::GenerateObject(string name, int values[4])
{
	int x = values[0];
	int y = values[1];
	int w = values[2];
	int h = values[3];

	VisibleGameObject* newObj = NULL;

	newObj = new Tile();
	
	newObj->Load(w, h);
	newObj->SetPosition(x, y);
	newObj->SetType(name);

	return newObj;
}

VisibleGameObject* ObjectFactory::GenerateCollisionBox(int values[4])
{
	int x = values[0];
	int y = values[1];
	int w = values[2];
	int h = values[3];

	VisibleGameObject* newObj = new VisibleGameObject;

	sf::Rect<float> collRect{0.f, 0.f, (float)w, (float)h };

	newObj->Load(w, h);
	newObj->SetPosition(x, y);
	newObj->SetSolid(true);
	newObj->SetCollisionRectangle(collRect);
	return newObj;
}