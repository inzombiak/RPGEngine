#include "InventoryComponent.h"
#include "ItemManager.h"

bool InventoryComponent::Init(XMLElement* node)
{
	if (node->QueryBoolAttribute("drop", &m_canDrop) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	if (node->QueryIntAttribute("maxsize", &m_maxSize) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	
	XMLElement* pItem = node->FirstChildElement("Item");
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
	ItemID itemID = reinterpret_cast<ItemComponentID>(HashedString::hash_name(itemName.c_str()));
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

void InventoryComponent::ToggleDisplay()
{
	std::map<ItemID, Item>::iterator iter;
	for (iter = m_heldItems.begin(); iter != m_heldItems.end(); ++iter)
	{
		iter->second.ToggleVisible();
	}
}

const char* InventoryComponent::COMPONENT_NAME = "InventoryComponent";