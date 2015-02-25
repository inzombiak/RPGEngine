#include "ObjectRenderer.h"

void ObjectRenderer::SetMovableObjects(std::vector<VisibleGameObject*>& objects)
{
	m_movableGameObjects = objects;
	Sort(m_movableGameObjects);
}

void ObjectRenderer::SetStaticObjects(std::vector<VisibleGameObject*>& objects)
{
	m_staticGameObjects = objects;
	Sort(m_staticGameObjects);
}

void ObjectRenderer::SetPlayer(VisibleGameObject* player)
{
	//m_player = player;
	m_staticGameObjects.push_back(player);
}

void ObjectRenderer::Draw(sf::RenderWindow& rw)
{	
	m_frameCount++;
	int i = 0;
	if (m_frameCount == m_maxFrameCount)
	{
		Sort(m_staticGameObjects);
		m_frameCount = 0;
	}

	for (i = 0; i < m_staticGameObjects.size(); ++i)
	{
		m_staticGameObjects[i]->Draw(rw);
	}
}

void ObjectRenderer::Sort(std::vector<VisibleGameObject*>& list)
{
	list = Algorithms<VisibleGameObject*>::MergeSortVector(list, CompareBottom);
}

void ObjectRenderer::Swap(VisibleGameObject*& obj1, VisibleGameObject*& obj2)
{
	VisibleGameObject* temp = obj1;
	obj1 = obj2;
	obj2 = temp;
}

bool ObjectRenderer::CompareBottom(VisibleGameObject*& obj1, VisibleGameObject*& obj2)
{
	bool result = (obj1->GetPosition().y + obj1->GetHeight()) < (obj2->GetPosition().y + obj2->GetHeight());
	return result;
}

bool ObjectRenderer::CompareVisibleGameObjectsBottom(const VisibleGameObject*& obj1, const VisibleGameObject*& obj2)
{

	bool result = (obj1->GetPosition().y + obj1->GetHeight()) < (obj2->GetPosition().y + obj2->GetHeight());
	return result;
}