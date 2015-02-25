#ifndef PHYSICS_H
#define PHYSICS_H

#include "custom_defininitions.h"
#include "SFML\Graphics.hpp"
#include <vector>

using std::vector;

class Physics
{
public:
	Physics();
	~Physics();

private:
	bool CheckCollision(sf::Rect<float>&, sf::Rect<float>&);
	int ResolveCollision(StrongEntityPtr, StrongEntityPtr);
	vector<StrongEntityPtr> GetPossibleCollisions(StrongEntityPtr);
};

#endif
