#include "ItemManager.h"
#include "..\Utilities\Debug.h"
#include "..\Utilities\tinyxml2.h"

#include "..\Components\ComponentFactory.h"
#include "..\Components\InventoryComponent.h"
#include "..\Components\EquipmentComponent.h"
#include "..\Components\PickupComponent.h"
#include "EntityManager.h"

using tinyxml2::XMLElement;

ItemManager::ItemManager()
{
	//Register item component creation functions with the ComponentFactory
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME), GenericDerivedCreationFunction<StrongItemComponentPtr,BaseItemComponent>);
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(RestorationItemComponent::COMPONENT_NAME), GenericDerivedCreationFunction<StrongItemComponentPtr, RestorationItemComponent>);
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(ConsumableItemComponent::COMPONENT_NAME), GenericDerivedCreationFunction<StrongItemComponentPtr, ConsumableItemComponent>);
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(ItemRenderComponent::COMPONENT_NAME), GenericDerivedCreationFunction<StrongItemComponentPtr, ItemRenderComponent>);;
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(EquipableItemComponent::COMPONENT_NAME), GenericDerivedCreationFunction<StrongItemComponentPtr, EquipableItemComponent>);;
}

StrongComponentPtr ItemManager::CreateInventoryComponent()
{
	return std::shared_ptr<InventoryComponent>(new InventoryComponent());
}

StrongComponentPtr ItemManager::CreateEquipmentComponent()
{
	return std::shared_ptr<EquipmentComponent>(new EquipmentComponent());
}

StrongComponentPtr ItemManager::CreateItemPickupComponent()
{
	return std::shared_ptr<ItemPickupComponent>(new ItemPickupComponent());
}

void ItemManager::LoadItemCatalog(string filepath)
{
	/*
	Error handling
	*/
	m_xmlFile.LoadFile(filepath.c_str());

	tinyxml2::XMLNode* pRoot = m_xmlFile.FirstChild();
	XMLElement* pItem = pRoot->FirstChildElement("Item");
	ItemDefinition newItem;
	while (pItem)
	{
		newItem.itemComponents.clear();
		string itemName = pItem->Attribute("name");
		newItem.name = itemName;
		if (InitializeItem(pItem, newItem))
			m_itemCatalog.insert(std::pair<ItemID, ItemDefinition>(reinterpret_cast<ItemComponentID>(HashedString::hash_name(itemName.c_str())), newItem));
		pItem = pItem->NextSiblingElement("Item");
	}
}

/*bool ItemManager::InitializeItem(XMLElement* pItem, ItemDefinition& item)
{
	XMLElement* pItemComponent = pItem->FirstChildElement("Component");
	StrongItemComponentPtr newItemComponent;
	while (pItemComponent)
	{
		newItemComponent = CreateItemComponent(pItemComponent);
		if (!newItemComponent)
			return false;
		else
		{
			if (newItemComponent->Init(pItemComponent))
			{
				item.itemComponents.push_back(newItemComponent);
			}
		}
		pItemComponent = pItemComponent->NextSiblingElement("Component");
	}
	return true;
}*/
bool ItemManager::InitializeItem(XMLElement* pItem, ItemDefinition& item)
{
	XMLElement* pItemComponent = pItem->FirstChildElement("Component");
	while (pItemComponent)
	{
		if (pItemComponent->Attribute("name"))
		{
			string componentName = pItemComponent->Attribute("name");
			ComponentID id = ItemComponent::GetIDFromName(componentName.c_str());
			item.itemComponents.push_back(std::pair<ComponentID, XMLElement*>(id, pItemComponent));
		}
		pItemComponent = pItemComponent->NextSiblingElement("Component");
	}
	return true;
}

StrongItemComponentPtr ItemManager::CreateItemComponent(XMLElement* node)
{
	StrongItemComponentPtr newComponent;

	if (node->Attribute("name"))
	{
		string componentName = node->Attribute("name");
		newComponent = StrongItemComponentPtr(ComponentFactory::CreateItemComponent(ItemComponent::GetIDFromName(componentName.c_str())));
	}

	return newComponent;
}
bool ItemManager::CreateItemByID(ItemID itemID, Item& item)
{
	auto it = m_itemCatalog.find(itemID);
	
	if (it == m_itemCatalog.end())
		return false;
	ItemDefinition& itemDef = it->second;
	StrongItemComponentPtr newComponent;
	for (int i = 0; i < itemDef.itemComponents.size(); ++i)
	{
		ComponentID id = itemDef.itemComponents[i].first;
		newComponent = StrongItemComponentPtr(ComponentFactory::CreateItemComponent(id));
		newComponent->Init(itemDef.itemComponents[i].second);
		item.AddItemComponent(id, newComponent);
	}
	return true;
}

bool ItemManager::DropItem(ItemID itemId, sf::Vector2f pos)
{
	Debug::PrintMessage("Dropping Item");
	//Error handling
	if (!m_entityManager)
		return false;
	//Change to force
	pos.x += 30;
	pos.y += 40;
	if (!m_entityManager->CreateEntityAtPosition(m_itemCatalog[itemId].name, pos))
		return false;
	return true;
}

ItemManager::ItemCatalog ItemManager::m_itemCatalog;
EntityManager* ItemManager::m_entityManager;