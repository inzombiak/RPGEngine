#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "..\Components\CollisionComponent.h"
#include "..\Utilities\GameDefs.h""
#include "SFML\Graphics.hpp"
#include <vector>

using std::vector;

class PhysicsManager
{
public:
	void Update(float dt);
	//Used for debug drawing
	void Draw(sf::RenderWindow& rw);
	static StrongComponentPtr CreateCollisionComponent();

private:
	bool CheckCollision(int collComponentIndex1, int collComponentIndex2);

	static vector<std::shared_ptr<CollisionComponent>> m_collisionComponents;
};

#endif
