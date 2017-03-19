#include "PhysicsManager.h"
#include "..\Utilities\Debug.h"
#include "PhysicsHelpers.h"

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
	PhysicsDefs::MTV result;
	//If were dealign with polygons, run SAT
	if (m_collisionComponents[collComponentIndex1]->GetCollisionShapeType() != PhysicsDefs::Circle
		&& m_collisionComponents[collComponentIndex2]->GetCollisionShapeType() != PhysicsDefs::Circle)
	{
		result = CollisionFunctions::SATTest(m_collisionComponents[collComponentIndex1]->GetCollisionShape(), m_collisionComponents[collComponentIndex2]->GetCollisionShape());
		if (result.didCollide)
			m_collisionComponents[collComponentIndex2]->UpdatePosition(sf::Vector2f(result.xyIncrement.x, result.xyIncrement.y));
	}
	
	return result.didCollide;
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