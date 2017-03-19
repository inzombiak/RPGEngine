#include "PickupComponent.h"
#include "..\Utilities\Debug.h"

#include "..\Entity.h"
#include "InventoryComponent.h"
#include "CollisionComponent.h"


#include <functional>

bool ItemPickupComponent::Init(XMLElement* node)
{
	if (!node->Attribute("itemname"))
		return false;
	m_itemName = node->Attribute("itemname");
	if (node->QueryIntAttribute("quantity", &m_quantity) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	if (node->QueryBoolAttribute("persistent", &m_persistent) != tinyxml2::XMLError::XML_SUCCESS)
		return false;

	return true;
}

void ItemPickupComponent::PostInit()
{
	//If owner has a collision component, add the Apply function to its list of reaction functions
	WeakComponentPtr weakCollisionPtr = m_owner->GetComponent(IComponent::GetIDFromName(CollisionComponent::COMPONENT_NAME));
	//If the target doesn't have an inventory
	if (weakCollisionPtr.expired())
		return;
	StrongComponentPtr strongCollisionPtr = ConvertToStrongPtr(weakCollisionPtr);
	std::shared_ptr<CollisionComponent> collision = CastComponentToDerived<StrongComponentPtr, CollisionComponent>(strongCollisionPtr);
	std::function<void(StrongEntityPtr)> f = std::bind(&ItemPickupComponent::Apply, this, std::placeholders::_1);
	collision->AddReactionFunction(f);
}

void ItemPickupComponent::Apply(StrongEntityPtr target)
{
	/*
	TODO
	Do items disappear if entity doesnt have an inventory?
	*/

	WeakComponentPtr weakInventoryPtr = target->GetComponent(IComponent::GetIDFromName(InventoryComponent::COMPONENT_NAME));
	//If the target doesn't have an inventory
	if (weakInventoryPtr.expired())
		return;
	Debug::PrintMessage("Adding item");
	StrongComponentPtr strongInventoryPtr = ConvertToStrongPtr(weakInventoryPtr);
	std::shared_ptr<InventoryComponent> inventory = CastComponentToDerived<StrongComponentPtr, InventoryComponent>(strongInventoryPtr);
	inventory->AddItem(m_itemName, m_quantity);
	m_owner->SetInUse(false);
}

const char* ItemPickupComponent::COMPONENT_NAME = "ItemPickupComponent";