#include "PhysicsManager.h"
#include "Debug.h"

StrongComponentPtr PhysicsManager::CreateCollisionComponent()
{
	std::shared_ptr<CollisionComponent> newCollisionComp(new CollisionComponent());
	m_collisionComponents.push_back(newCollisionComp);
	return newCollisionComp;
}

void PhysicsManager::Draw(sf::RenderWindow& rw)
{
	for (vector<std::shared_ptr<CollisionComponent>>::iterator it = m_collisionComponents.begin(); it != m_collisionComponents.end(); ++it)
	{
		(*it)->Draw(rw);
	}
}

void PhysicsManager::Update(float dt)
{
	//If componenet is no longer in use, we remove it
	for (vector<std::shared_ptr<CollisionComponent>>::iterator it = m_collisionComponents.begin(); it != m_collisionComponents.end();)
	{
		if (!(*it)->GetInUse())
			it = m_collisionComponents.erase(it);
		else
		{
			(*it)->Update(dt);
			++it;
		}	
	}

	for (int i = 0; i < m_collisionComponents.size(); ++i)
	{
		if (m_collisionComponents[i]->GetInUse())
			for (int j = i; j < m_collisionComponents.size(); ++j)
			{
				if (i != j)
					if(m_collisionComponents[j]->GetInUse())
						if (CheckCollision(i,j))
						{
							m_collisionComponents[i]->ApplyCollision(ConvertToStrongPtr(m_collisionComponents[j]->GetOwner()));
							Debug::PrintMessage("Collision");
							m_collisionComponents[j]->ApplyCollision(ConvertToStrongPtr(m_collisionComponents[i]->GetOwner()));
						}
			}
	}
}

bool PhysicsManager::CheckCollision(int collComponentIndex1, int collComponentIndex2)
{
	bool result;
	//If were dealign with polygons, run SAT
	if (m_collisionComponents[collComponentIndex1]->GetCollisionShape() != CollisionShape::Circle
		&& m_collisionComponents[collComponentIndex2]->GetCollisionShape() != CollisionShape::Circle)
		result = SATTest(collComponentIndex1, collComponentIndex2);
	return result;
}

bool PhysicsManager::SATTest(int collComponentIndex1, int collComponentIndex2)
{
	bool result = true;

	//Get the points of each shape
	auto p1Points = m_collisionComponents[collComponentIndex1]->GetBounds();
	auto p2Points = m_collisionComponents[collComponentIndex2]->GetBounds();

	//Minimum translation axis.
	sf::Vector2f mtAxis;
	//Minimum translation value. When united with mtAxis we get the minimum translation vector
	double mtValue = DBL_MAX;
	double temp;
	//If no points return
	if (p1Points.size() < 1 || p2Points.size() < 1)
		return false;

	//Generate axes to check for each shape
	std::vector<sf::Vector2f> p1Axes = GetAxes(p1Points), p2Axes = GetAxes(p2Points);

	Projection p1Proj, p2Proj;

	//After calculating the axes, we can begin checking for overlaps
	for (int i = 0; i < p1Axes.size(); ++i)
	{

		//Get projections
		p1Proj = GetProjection(p1Points, p1Axes[i]);
		p2Proj = GetProjection(p2Points, p1Axes[i]);

		//Check for overlap. If no overlap, then no interection between polygons so we return
		//Otherwise we compare to mtValue
		if (!DoProjectionsOverlap(p1Proj, p2Proj))
			return false;
		else
		{
			//Get value of overlap
			temp = CalculateOverlap(p1Proj, p2Proj);

			//If it is less than pervious, change mtValue and mtAxis accordingly
			if (temp < mtValue)
			{
				mtValue = temp;
				mtAxis = p1Axes[i];
			}
		}
	}

	//After calculating the axes, we can begin checking for overlaps
	for (int i = 0; i < p2Axes.size(); ++i)
	{

		//Get projections
		p1Proj = GetProjection(p1Points, p2Axes[i]);
		p2Proj = GetProjection(p2Points, p2Axes[i]);

		//Check for overlap. If no overlap, then no interection between polygons so we return
		//Otherwise we compare to mtValue
		if (!DoProjectionsOverlap(p1Proj, p2Proj))
			return false;
		else
		{
			//Get value of overlap
			temp = CalculateOverlap(p1Proj, p2Proj);

			//If it is less than pervious, change mtValue and mtAxis accordingly
			if (temp < mtValue)
			{
				mtValue = temp;
				mtAxis = p2Axes[i];
			}
		}
	}
	//If either of the collisions arent solid, not need to apply mtv
	if (!m_collisionComponents[collComponentIndex1]->GetIsSolid() || !m_collisionComponents[collComponentIndex2]->GetIsSolid())
		return true;
	//If we got this far then the objects are intersecting and we have an MTV.
	//We use this MTV to push objects out of on another, in this case p2 out of p1
	sf::Vector2f directionVector, MTV;

	//Calculate direction vector
	directionVector = sf::Vector2f(m_collisionComponents[collComponentIndex2]->GetPosition().x - m_collisionComponents[collComponentIndex1]->GetPosition().x,
		m_collisionComponents[collComponentIndex2]->GetPosition().y - m_collisionComponents[collComponentIndex1]->GetPosition().y);

	//Get MTV
	MTV = CalculateMTV(mtAxis, mtValue, directionVector);

	//Move object 1
//	m_collisionComponents[collComponentIndex1]->UpdatePosition(sf::Vector2f(MTV.x, MTV.y));
	m_collisionComponents[collComponentIndex2]->UpdatePosition(sf::Vector2f(MTV.x, MTV.y));

	return true;
}

PhysicsManager::Projection PhysicsManager::GetProjection(std::vector<sf::Vector2f> points, sf::Vector2f axis)
{
	Projection result;

	//Set min/max to the first point for a base case
	result.min = points[0].x * axis.x + points[0].y * axis.y;
	result.max = result.min;

	float newProj;

	for (int i = 1; i < points.size(); ++i)
	{
		//Get projection of the each point, change min/max if needed
		newProj = points[i].x * axis.x + axis.y * points[i].y;

		if (newProj < result.min)
			result.min = newProj;
		else if (newProj > result.max)
			result.max = newProj;
	}
	return result;
}

bool PhysicsManager::DoProjectionsOverlap(const PhysicsManager::Projection& proj1, const PhysicsManager::Projection& proj2)
{
	bool result = true;

	//If there is a gap, then no overlap
	if ((proj1.max <= proj2.min) || (proj2.max <= proj1.min))
		result = false;

	return result;
}

double PhysicsManager::CalculateOverlap(const PhysicsManager::Projection& proj1, const PhysicsManager::Projection& proj2)
{
	return std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
}

sf::Vector2f PhysicsManager::CalculateMTV(sf::Vector2f axis, double magnitude, sf::Vector2f directionVector)
{
	//Calculate slope of axis
	double angle = atan(axis.y / axis.x);

	//Calculate change along x and y axes
	sf::Vector2f result(cos(angle)*magnitude, sin(angle)*magnitude);

	//Check to make sure MTV isn't pointing towards a polygon
	if (result.x*directionVector.x + result.y * directionVector.y < 0)
	{
		result.x = -result.x;
		result.y = -result.y;
	}

	return result;
}

std::vector<sf::Vector2f> PhysicsManager::GetAxes(std::vector<sf::Vector2f> points)
{
	std::vector<sf::Vector2f> result;

	double length;
	sf::Vector2f axis, edge;
	for (int i = 0; i < points.size(); ++i)
	{
		//Calculate the edge between each point and its neighbor
		edge.x = points[(i + 1) % points.size()].x - points[i].x;
		edge.y = points[(i + 1) % points.size()].y - points[i].y;

		//Get length of edge
		length = sqrt(edge.x * edge.x + edge.y * edge.y);

		//Normalize
		edge.x /= length;
		edge.y /= length;

		//Push the pependiular vector to edge into the axes vector
		result.push_back(sf::Vector2f(-edge.y, edge.x));
	}

	return result;
}

vector<std::shared_ptr<CollisionComponent>> PhysicsManager::m_collisionComponents;

//void PhysicsManager::ResolveCollision(std::shared_ptr<CollisionComponent>& obj1, std::shared_ptr<CollisionComponent>& obj2)
//{
//	float obj1HalfHeight = obj1->GetBounds().height / 2;
//	float obj2HalfHeight = obj2->GetBounds().height / 2;
//
//	float obj1HalfWidth = obj1->GetBounds().width / 2;
//	float obj2HalfWidth = obj2->GetBounds().width / 2;
//
//	float obj1X = obj1->GetBounds().left + obj1HalfWidth;
//	float obj1Y = obj1->GetBounds().top + obj1HalfHeight;
//	float obj2X = obj2->GetBounds().left + obj2HalfWidth;
//	float obj2Y = obj2->GetBounds().top + obj2HalfHeight;
//
//	float xDistance = abs(obj1X - obj2X);
//	float yDistance = abs(obj1Y - obj2Y);
//
//	float xOverlap = obj1HalfWidth + obj2HalfWidth - xDistance;
//	float yOverlap = obj1HalfHeight + obj2HalfHeight - yDistance;
//
//	if (abs(yOverlap) < 0.001)
//		yOverlap = 0.001;
//
//	if (abs(xOverlap) > abs(yOverlap))
//	{
//		xOverlap = 0;
//		if (obj1Y < obj2Y)
//		{
//			yOverlap *= -1;
//			
//		}
//	}
//	else
//	{
//		yOverlap = 0;
//		if (obj1X < obj2X)
//		{
//			xOverlap *= -1;
//		}
//	}
// 	obj1->UpdatePosition(sf::Vector2f(xOverlap,yOverlap));
//}