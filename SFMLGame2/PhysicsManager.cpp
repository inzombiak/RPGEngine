#include "PhysicsManager.h"

StrongComponentPtr PhysicsManager::CreateCollisionComponent()
{
	std::shared_ptr<CollisionComponent> newCollisionComp(new CollisionComponent());
	m_collisionComponents.push_back(newCollisionComp);
	return newCollisionComp;
}

void PhysicsManager::Update(float dt)
{
	for (int i = 0; i < m_collisionComponents.size(); ++i)
	{
		m_collisionComponents[i]->Update(dt);
	}

	for (int i = 0; i < m_collisionComponents.size(); ++i)
	{
		for (int j = i + 1; j < m_collisionComponents.size(); ++j)
		{
			if (CheckCollision(m_collisionComponents[i]->GetBounds(), m_collisionComponents[j]->GetBounds()))
			{
				if (m_collisionComponents[j]->GetIsSolid())
					ResolveCollision(m_collisionComponents[i], m_collisionComponents[j]);
				//m_collisionComponents[i]->ApplyCollision(ConvertToStrongPtr(m_collisionComponents[j]->GetOwner()));
				m_collisionComponents[j]->ApplyCollision(ConvertToStrongPtr(m_collisionComponents[i]->GetOwner()));
			}
				
		}
	}
}

bool PhysicsManager::CheckCollision(sf::Rect<float>& r1, sf::Rect<float>& r2)
{
	bool result = r1.intersects(r2);
	return result;
}


//vector<StrongEntityPtr*> GameObjectManagerNew::GetPossibleCollisions(StrongEntityPtr& object)
//{
//	//vector<int> possibleCollTileIndicies = LevelLoader::PossibleTiles(object.GetPosition());
//	vector<StrongEntityPtr*> result;
//
//	/*while (!possibleCollTileIndicies.empty())
//	{
//	result.push_back(m_levelTiles[possibleCollTileIndicies.back()]);
//	possibleCollTileIndicies.pop_back();
//	}*/
//
//	return result;
//}

void PhysicsManager::ResolveCollision(std::shared_ptr<CollisionComponent>& obj1, std::shared_ptr<CollisionComponent>& obj2)
{
	float obj1HalfHeight = obj1->GetBounds().height / 2;
	float obj2HalfHeight = obj2->GetBounds().height / 2;

	float obj1HalfWidth = obj1->GetBounds().width / 2;
	float obj2HalfWidth = obj2->GetBounds().width / 2;

	float obj1X = obj1->GetBounds().left + obj1HalfWidth;
	float obj1Y = obj1->GetBounds().top + obj1HalfHeight;
	float obj2X = obj2->GetBounds().left + obj2HalfWidth;
	float obj2Y = obj2->GetBounds().top + obj2HalfHeight;

	float xDistance = abs(obj1X - obj2X);
	float yDistance = abs(obj1Y - obj2Y);

	float xOverlap = obj1HalfWidth + obj2HalfWidth - xDistance;
	float yOverlap = obj1HalfHeight + obj2HalfHeight - yDistance;

	if (abs(yOverlap) < 0.001)
		yOverlap = 0.001;

	if (abs(xOverlap) > abs(yOverlap))
	{
		xOverlap = 0;
		if (obj1Y < obj2Y)
		{
			yOverlap *= -1;
			
		}
	}
	else
	{
		yOverlap = 0;
		if (obj1X < obj2X)
		{
			xOverlap *= -1;
		}
	}
	obj1->UpdatePosition(sf::Vector2f(obj1->GetBounds().left + xOverlap, obj1->GetBounds().top + yOverlap));
}

//void PhysicsManager::ResolveCollision(std::shared_ptr<CollisionComponent>& obj1, std::shared_ptr<CollisionComponent>& obj2)
//{
//	float obj1HalfHeight = obj1.GetCollisionRectangle().height / 2;
//	float obj2HalfHeight = obj2.GetCollisionRectangle().height / 2;
//
//	float obj1HalfWidth = obj1.GetCollisionRectangle().width / 2;
//	float obj2HalfWidth = obj2.GetCollisionRectangle().width / 2;
//
//	float obj1X = obj1.GetPosition().x + obj1.GetCollisionRectangle().left + obj1HalfWidth;
//	float obj1Y = obj1.GetPosition().y + obj1.GetCollisionRectangle().top + obj1HalfHeight;
//	float obj2X = obj2.GetPosition().x + obj2.GetCollisionRectangle().left + obj2HalfWidth;
//	float obj2Y = obj2.GetPosition().y + obj2.GetCollisionRectangle().top + obj2HalfHeight;
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
//	int result = -1;
//
//	if (abs(xOverlap) > abs(yOverlap))
//	{
//		xOverlap = 0;
//		if (obj1Y < obj2Y)
//		{
//			yOverlap *= -1;
//			result = 0;
//		}
//		else
//			result = 1;
//	}
//	else
//	{
//		result = 3;
//		yOverlap = 0;
//		if (obj1X < obj2X)
//		{
//			xOverlap *= -1;
//		}
//		else
//		{
//		}
//	}
//	obj1.SetPosition(obj1.GetPosition().x + xOverlap, obj1.GetPosition().y + yOverlap);
//	return result;
//}

vector<std::shared_ptr<CollisionComponent>> PhysicsManager::m_collisionComponents;