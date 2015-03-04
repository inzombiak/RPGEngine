#include "EntityManager.h"
#include "InputComponent.h"
#include "tinyxml2.h"
#include "Entity.h"
#include "Algorithms.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "EntityFactory.h"

EntityManager::EntityManager()
{

}

void EntityManager::Update(float time)
{
	//ZSortEntities();
}

void EntityManager::Move(float time)
{
	////m_Player->Move(1, 1, time);

	//for (vector<VisibleEntity*>::iterator it = m_staticEntitys.begin(); it != m_staticEntitys.end(); ++it)
	//{
	//	if ((*it)->GetSolid())
	//	{
	//		//if (CheckCollision(m_Player->GetGlobalCollisionRectangle(), (*it)->GetGlobalCollisionRectangle()))
	//		//{
	//		//	//m_Player->Stop();
	//		//	int r = ResolveCollision((*m_Player), *(*it));
	//		//	if (r == 0)
	//		//		m_Player->SetGrounded(true);
	//		//	else if (r == 1)
	//		//		m_Player->SetVelocityY(0);
	//		//}
	//	}
	//}

	////m_Player->Move(1, 0, time);

	//for (vector<VisibleEntity*>::iterator it = m_staticEntitys.begin(); it != m_staticEntitys.end(); ++it)
	//{
	//	if ((*it)->GetSolid())
	//	{
	//		/*if (CheckCollision(m_Player->GetGlobalCollisionRectangle(), (*it)->GetGlobalCollisionRectangle()))
	//		{
	//		ResolveCollision((*m_Player), *(*it));
	//		m_Player->SetVelocityX(0);
	//		}*/
	//	}
	//}
}

StrongEntityPtr EntityManager::CreateEntity()
{
	StrongEntityPtr entity(new Entity());
	m_entities.push_back(entity);
	return entity;
}

void EntityManager::LoadEntities(string filename)
{

	string f = "data/Entities.xml";

	tinyxml2::XMLDocument file;
	if (file.LoadFile(f.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		return;

	tinyxml2::XMLNode* pRoot = file.FirstChild();
	XMLElement* pEntities = pRoot->FirstChildElement("Entities");
	XMLElement* pEntity = pEntities->FirstChildElement("Entity");
	StrongEntityPtr newEntity;
	EntityFactory* em = EntityFactory::GetInstance();
	while (pEntity != nullptr)
	{
		newEntity = CreateEntity();
		em->CreateEntity(pEntity, newEntity);
		pEntity = pEntity->NextSiblingElement("Entity");
	}
}

//void EntityManager::SetForegroundTiles(vector<StrongEntityPtr>& objects)
//{
//	//m_entities.reserve(m_entities.size() + objects.size());
//	//m_entities.insert(m_entities.end(), objects.begin(), objects.end());
//}
//
//void EntityManager::SetBackgroundTiles(vector<StrongEntityPtr>& objects)
//{
//	//m_backgroundTiles = objects;
//}
//
//void EntityManager::SetCollisionBoxes(vector<StrongEntityPtr>& tiles)
//{
//	//m_colllisionBoxes = tiles;
//}

//void EntityManager::ZSortEntities()
//{
//	m_framesSinceLastSort++;
//
//	if (m_framesSinceLastSort == MAX_SORT_FRAME_INTERVAL)
//	{
//		m_entities = Algorithms<StrongEntityPtr>::MergeSortVector(m_entities, CompareBottom);
//		m_framesSinceLastSort = 0;
//	}
//}
//
//bool EntityManager::CompareBottom(StrongEntityPtr& entity1, StrongEntityPtr& entity2)
//{
//	ComponentID id = ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME);
//	std::shared_ptr<RenderComponent> rc1 = std::static_pointer_cast<RenderComponent>(StrongComponentPtr(entity1->GetComponent(id)));
//	std::shared_ptr<RenderComponent> rc2 = std::static_pointer_cast<RenderComponent>(StrongComponentPtr(entity2->GetComponent(id)));
//	if (rc1 && rc2)
//	{
//		bool result = (rc1->GetSprite().getPosition().y + rc1->GetSprite().getGlobalBounds().height) < (rc2->GetSprite().getPosition().y + rc2->GetSprite().getGlobalBounds().height);
//
//		return result;
//	}
//	else
//		return false;
//}

//int EntityManagerNew::ResolveCollision(StrongEntityPtr& obj1, StrongEntityPtr& obj2)
//{
//	/*float obj1HalfHeight = obj1.GetCollisionRectangle().height / 2;
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
//	return result;*/
//}

//bool EntityManagerNew::CheckCollision(sf::Rect<float>& r1, sf::Rect<float>& r2)
//{
//	bool result = r1.intersects(r2);
//	return result;
//}

//vector<StrongEntityPtr*> EntityManagerNew::GetPossibleCollisions(StrongEntityPtr& object)
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

//void EntityManager::Draw(sf::RenderWindow &rw)
//{
//	for (int i = 0; i < m_backgroundTiles.size(); ++i)
//	{
//		std::shared_ptr<RenderComponent> rc = std::static_pointer_cast<RenderComponent>(StrongComponentPtr(m_backgroundTiles[i]->GetComponent(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME))));
//		if (!m_backgroundTiles[i]->GetComponent(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME)).expired())
//		{
//			sf::Sprite s = rc->GetSprite();
//			rw.draw(s);
//		}
//	}
//	
//	for (int j = 0; j < m_entities.size(); ++j)
//	{
//		std::shared_ptr<RenderComponent> rc = std::static_pointer_cast<RenderComponent>(StrongComponentPtr(m_entities[j]->GetComponent(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME))));
//		if (!m_entities[j]->GetComponent(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME)).expired())
//		{
//			sf::Sprite s = rc->GetSprite();
//			rw.draw(s);
//		}
//	}
//	
//	//m_renderer.Draw(rw);
//}
