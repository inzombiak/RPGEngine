#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "CollisionComponent.h"

#include "custom_defininitions.h"
#include "SFML\Graphics.hpp"
#include <vector>

using std::vector;

class PhysicsManager
{
public:

	void Update(float dt);

	static StrongComponentPtr CreateCollisionComponent();
private:
	bool CheckCollision(sf::Rect<float>&, sf::Rect<float>&);
	void ResolveCollision(std::shared_ptr<CollisionComponent>&, std::shared_ptr<CollisionComponent>&);
	vector<StrongEntityPtr> GetPossibleCollisions(StrongEntityPtr);
	static vector<std::shared_ptr<CollisionComponent>> m_collisionComponents;
};

#endif
