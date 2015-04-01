#include "InventoryComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "ItemManager.h"

bool InventoryComponent::Init(XMLElement* node)
{
	if (node->QueryBoolAttribute("drop", &m_canDrop) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	if (node->QueryIntAttribute("maxsize", &m_maxSize) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	XMLElement* pItems = node->FirstChildElement("Items");
	XMLElement* pItem = pItems->FirstChildElement("Item");
	while (pItem)
	{
		if (!AddItem(pItem->Attribute("name"), pItem->IntAttribute("quantity")))
			return false;
		pItem = pItem->NextSiblingElement("Item");
	}
	return true;
}

bool InventoryComponent::AddItem(string itemName, int quantity)
{
	ItemID itemID = reinterpret_cast<ItemID>(HashedString::hash_name(itemName.c_str()));
	std::map<ItemID, Item>::iterator it = m_heldItems.find(itemID);
	if (it == m_heldItems.end())
	{
		Item newItem;
		if (!ItemManager::CreateItemByID(itemID, newItem))
			return false;
		ItemComponentID itemCompID = ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME);
		StrongItemComponentPtr itemComp = ConvertToStrongPtr<ItemComponent>(newItem.GetItemComponent(itemCompID));
		if (!itemComp)
			return false;
		std::shared_ptr<BaseItemComponent> strongItemComp = CastComponentToDerived<StrongItemComponentPtr, BaseItemComponent>(itemComp);
		strongItemComp->SetQuantity(quantity);
		m_heldItems.insert(std::pair<ItemID, Item>(itemID, newItem));
	}
	else
	{
		WeakItemComponentPtr weakPtr = (it)->second.GetItemComponent(ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME));
		if (weakPtr.expired())
			return false;
		StrongItemComponentPtr itemComp = ConvertToStrongPtr<ItemComponent>(weakPtr);
		std::shared_ptr<BaseItemComponent> strongItemComp = CastComponentToDerived<StrongItemComponentPtr, BaseItemComponent>(itemComp);
		strongItemComp->SetQuantity(strongItemComp->GetQuantity() + quantity);
	}
	return true;
}

bool InventoryComponent::AddItem(Item newItem, int quantity)
{
	std::shared_ptr<BaseItemComponent> strongItemComp;
	if (CheckConvertAndCastPtr<ItemComponent, BaseItemComponent>(newItem.GetItemComponent(ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME)), strongItemComp))
	{
		ItemID itemID = reinterpret_cast<ItemID>(HashedString::hash_name(strongItemComp->GetItemName().c_str()));
		std::map<ItemID, Item>::iterator it = m_heldItems.find(itemID);
		if (it == m_heldItems.end())
		{
			strongItemComp->SetQuantity(quantity);
			m_heldItems.insert(std::pair<ItemID, Item>(itemID, newItem));
		}
		else
		{
			if (CheckConvertAndCastPtr<ItemComponent, BaseItemComponent>(it->second.GetItemComponent(ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME)), strongItemComp))
			{
				strongItemComp->SetQuantity(strongItemComp->GetQuantity() + quantity);
			}	
		}
	}
	return true;
}

void InventoryComponent::ToggleDisplay()
{
	std::map<ItemID, Item>::iterator iter;
	for (iter = m_heldItems.begin(); iter != m_heldItems.end(); ++iter)
	{
		//iter->second.ToggleVisible();
	}
}

void InventoryComponent::UseItem(ItemID itemId)
{
	auto iter = m_heldItems.find(itemId);
	if (iter == m_heldItems.end())
		return;
	Item usedItem = iter->second; 
	
	ItemComponentID itemCompID = ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME);
	StrongItemComponentPtr itemComp = ConvertToStrongPtr<ItemComponent>(usedItem.GetItemComponent(itemCompID));
	if (!itemComp)
		return;
	
	std::shared_ptr<BaseItemComponent> strongItemComp = CastComponentToDerived<StrongItemComponentPtr, BaseItemComponent>(itemComp);
	if (strongItemComp->GetQuantity() == 1)
		m_heldItems.erase(iter);
	else
		strongItemComp->SetQuantity(strongItemComp->GetQuantity() - 1);
	usedItem.Apply(m_owner);	
}

void InventoryComponent::DropItem(ItemID itemId)
{
	auto iter = m_heldItems.find(itemId);
	if (iter == m_heldItems.end())
		return;
	Item& dropItem = iter->second;

	ItemComponentID itemCompID = ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME);
	StrongItemComponentPtr itemComp = ConvertToStrongPtr<ItemComponent>(dropItem.GetItemComponent(itemCompID));
	if (!itemComp)
		return;
	std::shared_ptr<BaseItemComponent> strongItemComp = CastComponentToDerived<StrongItemComponentPtr, BaseItemComponent>(itemComp);
	std::shared_ptr<TransformComponent> entityLoc;
		//Error handling
	if (!CheckConvertAndCastPtr<ComponentBase, TransformComponent>(m_owner->GetComponent(ComponentBase::GetIDFromName(TransformComponent::COMPONENT_NAME)), entityLoc))
		return;
	ItemManager::DropItem(itemId, entityLoc->GetPosition());
	if (strongItemComp->GetQuantity() == 1)
		m_heldItems.erase(iter);
	else
		strongItemComp->SetQuantity(strongItemComp->GetQuantity() - 1);
}

void InventoryComponent::DeleteItem(ItemID itemId)
{
	auto iter = m_heldItems.find(itemId);
	if (iter == m_heldItems.end())
		return;
	Item& usedItem = iter->second;

	ItemComponentID itemCompID = ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME);
	StrongItemComponentPtr itemComp = ConvertToStrongPtr<ItemComponent>(usedItem.GetItemComponent(itemCompID));
	if (!itemComp)
		return;
	std::shared_ptr<BaseItemComponent> strongItemComp = CastComponentToDerived<StrongItemComponentPtr, BaseItemComponent>(itemComp);
	if (strongItemComp->GetQuantity() == 1)
		m_heldItems.erase(iter);
	else
		strongItemComp->SetQuantity(strongItemComp->GetQuantity() - 1);
}

const std::map<ItemID, Item>& InventoryComponent::GetItems()
{
	return m_heldItems;
}

const char* InventoryComponent::COMPONENT_NAME = "InventoryComponent";