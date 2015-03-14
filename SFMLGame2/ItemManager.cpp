#include "ItemManager.h"
#include "tinyxml2.h"

#include "ComponentFactory.h"
#include "InventoryComponent.h"
#include "PickupComponent.h"
#include "RenderComponent.h"

using tinyxml2::XMLElement;

ItemManager::ItemManager()
{
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME), ItemComponentCreationFunction<BaseItemComponent>);
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(RestorationItemComponent::COMPONENT_NAME), ItemComponentCreationFunction<RestorationItemComponent>);
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(ConsumableItemComponent::COMPONENT_NAME), ItemComponentCreationFunction<ConsumableItemComponent>);
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(ItemRenderComponent::COMPONENT_NAME), ItemComponentCreationFunction<ItemRenderComponent>);
}

StrongComponentPtr ItemManager::CreateInventoryComponent()
{
	return std::shared_ptr<InventoryComponent>(new InventoryComponent());
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
		if (InitializeItem(pItem, newItem))
			m_itemCatalog.insert(std::pair<ItemID, ItemDefinition>(reinterpret_cast<ItemComponentID>(HashedString::hash_name(itemName.c_str())), newItem));
		pItem = pItem->NextSiblingElement("Item");
	}
}

bool ItemManager::InitializeItem(XMLElement* pItem, ItemDefinition& item)
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
	XMLElement* pRenderComponent = pItem->FirstChildElement("RenderComponent");
	if (pRenderComponent)
	{
		item.renderComponenetInfo = pRenderComponent;
	}
	return true;
}

StrongItemComponentPtr ItemManager::CreateItemComponent(XMLElement* node)
{
	StrongItemComponentPtr newComponent;
	string what = node->Attribute("type");
	if (node->Attribute("type"))
	{
		string componentName = node->Attribute("type");
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
	for (int i = 0; i < itemDef.itemComponents.size(); ++i)
	{
		string name = itemDef.itemComponents[i]->GetName();
		item.AddItemComponent(ItemComponent::GetIDFromName(name.c_str()) ,itemDef.itemComponents[i]);
	}
	//Create Rendercomponent
	if (!itemDef.renderComponenetInfo)
		return false;
	string test = itemDef.renderComponenetInfo->Attribute("filepath");
	StrongComponentPtr renderComp = ComponentFactory::CreateEntityComponent(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME));
	if (!renderComp->Init(itemDef.renderComponenetInfo))
		return false;
	item.AddRenderComponent(renderComp);
	return true;
}

ItemManager::ItemCatalog ItemManager::m_itemCatalog;