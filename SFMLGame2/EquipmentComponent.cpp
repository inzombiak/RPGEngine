#include "EquipmentComponent.h"
#include "ItemManager.h"
#include "StatComponent.h"
#include "Entity.h"

bool EquipmentComponent::Init(XMLElement* node)
{
	XMLElement* pSlots = node->FirstChildElement("Slots");
	XMLElement* pSlot = pSlots->FirstChildElement("Slot");

	while (pSlot)
	{
		if (!pSlot->Attribute("name"))
			return false;

		m_availableSlots[Equipment::slotMap[pSlot->Attribute("name")]] = true;
		if (pSlot->Attribute("itemname"))
		{
			ItemID itemID = reinterpret_cast<ItemID>(HashedString::hash_name(pSlot->Attribute("itemname")));
			Item newItem;
			ItemManager::CreateItemByID(itemID, newItem);
			m_equipment[Equipment::slotMap[pSlot->Attribute("name")]] = newItem;
		}

		
		pSlot = pSlot->NextSiblingElement("Slot");
	}

	return true;
}

void EquipmentComponent::PostInit()
{
	for (std::map<Equipment::SlotName, Item>::iterator it = m_equipment.begin(); it != m_equipment.end(); ++it)
		ApplyModifiers(it->second);
}

bool EquipmentComponent::HasSlot(Equipment::SlotName slot)
{
	auto it = m_availableSlots.find(slot);
	if (it != m_availableSlots.end())
		return true;
	return false;
}

bool EquipmentComponent::Equip(Equipment::SlotName slot, Item newItem, Item& prevItem)
{
	if (!HasSlot(slot))
		return false;

	bool response = false;
	auto it = m_equipment.find(slot);
	if (it != m_equipment.end())
	{
		Unequip(slot, prevItem);
		response = true;
	}
	ApplyModifiers(newItem);
	m_equipment[slot] = newItem;
	return response;
}

bool EquipmentComponent::Unequip(Equipment::SlotName slot, Item& item)
{
	if (!HasSlot(slot))
		return false;

	auto it = m_equipment.find(slot);
	if (it == m_equipment.end())
		return false;
	item = it->second;
	RemoveModifiers(item);
	m_equipment.erase(it);
	return true;
}

void EquipmentComponent::ApplyModifiers(Item& newItem)
{
	//Check if component has an owner
	if (!m_owner)
		return;

	//Get statComponent
	std::shared_ptr<StatComponent> entityStatsComp;
	if (!CheckConvertAndCastPtr<ComponentBase, StatComponent>(m_owner->GetComponent(ComponentBase::GetIDFromName(StatComponent::COMPONENT_NAME)), entityStatsComp))
		return;

	//Get equipableItemComponent to access item modifiers
	std::shared_ptr<EquipableItemComponent> equipableItemComp;
	if (!CheckConvertAndCastPtr<ItemComponent, EquipableItemComponent>(newItem.GetItemComponent(ItemComponent::GetIDFromName(EquipableItemComponent::COMPONENT_NAME)), equipableItemComp))
		return;

	std::map<Stats::StatName, double> modifierMap = equipableItemComp->GetAllModifiers();
	for (std::map<Stats::StatName, double>::iterator it = modifierMap.begin(); it != modifierMap.end(); ++it)
	{
		entityStatsComp->SetStat(it->first, entityStatsComp->GetStat(it->first) + it->second);
	}
}

void EquipmentComponent::RemoveModifiers(Item& oldItem)
{
	//Check if component has an owner
	if (!m_owner)
		return;

	//Get statComponent
	std::shared_ptr<StatComponent> entityStatsComp;
	if (!CheckConvertAndCastPtr<ComponentBase, StatComponent>(m_owner->GetComponent(ComponentBase::GetIDFromName(StatComponent::COMPONENT_NAME)), entityStatsComp))
		return;

	//Get equipableItemComponent to access item modifiers
	std::shared_ptr<EquipableItemComponent> equipableItemComp;
	if (!CheckConvertAndCastPtr<ItemComponent, EquipableItemComponent>(oldItem.GetItemComponent(ItemComponent::GetIDFromName(EquipableItemComponent::COMPONENT_NAME)), equipableItemComp))
		return;

	std::map<Stats::StatName, double> modifierMap = equipableItemComp->GetAllModifiers();
	for (std::map<Stats::StatName, double>::iterator it = modifierMap.begin(); it != modifierMap.end(); ++it)
	{
		entityStatsComp->SetStat(it->first, entityStatsComp->GetStat(it->first) - it->second);
	}
}

const char* EquipmentComponent::COMPONENT_NAME = "EquipmentComponent";