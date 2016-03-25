#include "..\Utilities\Algorithms.h"

#include "..\Entity.h"
#include "..\Systems\EntityManager.h"
#include "..\Components\InputComponent.h"
#include "..\Components\RenderComponent.h"
#include "..\Components\TransformComponent.h"
#include "..\Components\VitalsComponent.h"
#include "..\Components\StatComponent.h"

#include "EntityFactory.h"

EntityManager::EntityManager()
{

}

void EntityManager::Update(float time)
{
	for (vector<std::shared_ptr<VitalsComponent>>::iterator it = m_entityVitals.begin(); it != m_entityVitals.end();)
	{
		if (!(*it)->GetInUse())
			it = m_entityVitals.erase(it);
		else
			++it;
	}

	for (vector<std::shared_ptr<Entity>>::iterator it = m_entities.begin(); it != m_entities.end();)
	{
		if (!(*it)->GetInUse())
			it = m_entities.erase(it);
		else
			++it;
	}
}

StrongComponentPtr EntityManager::CreateVitalsComponent()
{
	std::shared_ptr<VitalsComponent> newVitalsComp(new VitalsComponent());
	m_entityVitals.push_back(newVitalsComp);
	return newVitalsComp;
}

StrongComponentPtr EntityManager::CreateStatComponent()
{
	std::shared_ptr<StatComponent> newStatComp(new StatComponent());
	m_entityStats.push_back(newStatComp);
	return newStatComp;
}

StrongEntityPtr EntityManager::CreateEntity()
{
	StrongEntityPtr entity(new Entity());
	entity->SetInUse(true);
	m_entities.push_back(entity);
	return entity;
}

bool EntityManager::CreateEntityAtPosition(const string& entityName, sf::Vector2f pos)
{
	StrongEntityPtr newEntity = CreateEntity();
	EntityFactory* entityFactory = EntityFactory::GetInstance();
	if (entityFactory->CreateEntity(m_entityDefinitions[entityFactory->GetEntityNameIDFromName(entityName)], newEntity))
	{
		std::shared_ptr<TransformComponent> newEntityTrans;
		if (CheckConvertAndCastPtr<IComponent, TransformComponent>(newEntity->GetComponent(IComponent::GetIDFromName(TransformComponent::COMPONENT_NAME)), newEntityTrans))
		{
			newEntityTrans->SetPosition(pos);
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

void EntityManager::LoadEntities(string filename)
{
	string f = "data/Entities.xml";
	if (m_entityDefinitionFile.LoadFile(f.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		return;

	tinyxml2::XMLNode* pRoot = m_entityDefinitionFile.FirstChild();
	XMLElement* pEntities = pRoot->FirstChildElement("Entities");
	XMLElement* pEntity = pEntities->FirstChildElement("Entity");
	StrongEntityPtr newEntity;
	EntityFactory* entityFactory = EntityFactory::GetInstance();
	while (pEntity != nullptr)
	{
		newEntity = CreateEntity();
		entityFactory->CreateEntity(pEntity, newEntity);
		EntityNameID nameID = newEntity->GetNameID();
		std::pair<EntityNameID, tinyxml2::XMLElement*> newEntry(nameID, pEntity);
		m_entityDefinitions.insert(newEntry);
		pEntity = pEntity->NextSiblingElement("Entity");
	}
}
//bool EntityManager::CompareBottom(StrongEntityPtr& entity1, StrongEntityPtr& entity2)
//{
//	ComponentID id = IComponent::GetIDFromName(RenderComponent::COMPONENT_NAME);
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

vector<std::shared_ptr<VitalsComponent>> EntityManager::m_entityVitals;
vector<std::shared_ptr<StatComponent>> EntityManager::m_entityStats;