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
	//Used for debug drawing
	void Draw(sf::RenderWindow& rw);
	static StrongComponentPtr CreateCollisionComponent();

private:
	bool CheckCollision(int collComponentIndex1, int collComponentIndex2);

	//Methods for seperating axis theorem
	bool SATTest(int collComponentIndex1, int collComponentIndex2);
	//Struct for polygon projection
	struct Projection
	{
		float min, max;
	};
	//Returns vector of SAT axes. Arguement is the points of the shape
	std::vector<sf::Vector2f> GetAxes(std::vector<sf::Vector2f> points);
	//Get projection of a polygon on an axis
	Projection GetProjection(std::vector<sf::Vector2f> poly, sf::Vector2f axis);
	//Checks if projections overlap.
	bool DoProjectionsOverlap(const Projection& proj1, const Projection& proj2);
	//Returns overlap of two projections
	double CalculateOverlap(const Projection& proj1, const Projection& proj2);
	//Returns the MTV. Takes MTV axis, magnitude and the direction vector between the two polygons
	sf::Vector2f CalculateMTV(sf::Vector2f axis, double magnitude, sf::Vector2f directionVec);


	static vector<std::shared_ptr<CollisionComponent>> m_collisionComponents;
};

#endif
