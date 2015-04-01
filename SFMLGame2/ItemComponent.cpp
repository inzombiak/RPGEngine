#include "ItemComponent.h"
#include "EquipmentComponent.h"
#include "RenderComponent.h"
#include "VitalsComponent.h"
#include "InventoryComponent.h"
#include "Entity.h"

bool Item::AddItemComponent(ItemComponentID itemID, StrongItemComponentPtr compPtr)
{
	auto it = m_itemComponents.find(itemID);

	if (it != m_itemComponents.end())
		return false;
	m_itemComponents.insert(std::pair<ItemComponentID, StrongItemComponentPtr>(itemID, compPtr));
	return true;
}
WeakItemComponentPtr Item::GetItemComponent(ItemComponentID itemID)
{
	auto it = m_itemComponents.find(itemID);

	if (it == m_itemComponents.end())
		return WeakItemComponentPtr();
	return it->second;
}

void Item::Apply(StrongEntityPtr target)
{
	/*
	Need to fix logic separate equip and use
	*/
	auto iter = m_itemComponents.find(ItemComponent::GetIDFromName(RestorationItemComponent::COMPONENT_NAME));
	if (iter != m_itemComponents.end())
	{
		std::shared_ptr<RestorationItemComponent> restoItem = CastComponentToDerived<StrongItemComponentPtr, RestorationItemComponent>(iter->second);
		std::shared_ptr<VitalsComponent> targetVitals;
		if (CheckConvertAndCastPtr<ComponentBase, VitalsComponent>(target->GetComponent(ComponentBase::GetIDFromName(VitalsComponent::COMPONENT_NAME)), targetVitals))
		{
			VitalsComponent::VitalType type = targetVitals->StringtoVitalType(restoItem->GetEffect());
			VitalsComponent::Vital vital;
			if (type != VitalsComponent::VitalType::NaN)
			{
				if (targetVitals->GetVital(type, vital))
				{
					vital.current += restoItem->GetAmount();
					targetVitals->SetVitalCurrent(type, vital.current);
				}
			}
		}
	}
	else
	{
		/*Make more elegant*/
		iter = m_itemComponents.find(ItemComponent::GetIDFromName(EquipableItemComponent::COMPONENT_NAME));
		if (iter == m_itemComponents.end())
			return;
		std::shared_ptr<EquipableItemComponent> equipItem = CastComponentToDerived<StrongItemComponentPtr, EquipableItemComponent>(iter->second);
		std::shared_ptr<EquipmentComponent> equipComp;
		if (CheckConvertAndCastPtr<ComponentBase, EquipmentComponent>(target->GetComponent(ComponentBase::GetIDFromName(EquipmentComponent::COMPONENT_NAME)), equipComp))
		{
			Item oldItem;
			if (equipComp->Equip(equipItem->GetSlot(), (*this), oldItem))
			{
				std::shared_ptr<InventoryComponent> invComp;
				if (CheckConvertAndCastPtr<ComponentBase, InventoryComponent>(target->GetComponent(ComponentBase::GetIDFromName(InventoryComponent::COMPONENT_NAME)), invComp))
				{
					invComp->AddItem(oldItem, 1);
				}
			}
		}
	}
}

bool ItemRenderComponent::Init(const XMLElement* componentNode)
{
	sf::Texture tex;
	if (!tex.loadFromFile(componentNode->Attribute("filepath")))
	{
		//TODO add error message
		return false;
	}
	SetTexture(tex, sf::Rect<int>(0, 0, tex.getSize().x, tex.getSize().y));
	if (componentNode->QueryBoolAttribute("visible", &m_visible) != tinyxml2::XMLError::XML_SUCCESS)
	{
		//TODO add error message
		return false;
	}
	return true;
}

bool BaseItemComponent::Init(const XMLElement* componentNode)
{
	if (!componentNode->Attribute("name"))
		return false;
	m_name = componentNode->Attribute("name");
	if (!componentNode->Attribute("description"))
		return false;
	m_description = componentNode->Attribute("description");
	/*if (!componentNode->Attribute("iconfilepath"))
		return false;
		m_iconFilepath = componentNode->Attribute("iconfilepath");*/
	return true;

}

bool RestorationItemComponent::Init(const XMLElement* componentNode)
{
	if (!componentNode->Attribute("effect"))
		return false;
	m_effect = componentNode->Attribute("effect");
	if (componentNode->QueryIntAttribute("amount", &m_amount) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	return true;
}

bool ConsumableItemComponent::Init(const XMLElement* componentNode)
{
	if (componentNode->QueryIntAttribute("uses", &m_uses) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	if (componentNode->QueryIntAttribute("maxstack", &m_maxStack) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	return true;
}

bool EquipableItemComponent::Init(const XMLElement* componentNode)
{
	if (!componentNode->Attribute("slot"))
		return false;
	m_slot = Equipment::slotMap[componentNode->Attribute("slot")];
	const XMLElement* pModifiers = componentNode->FirstChildElement("Modifiers");
	const XMLElement* pModifier = pModifiers->FirstChildElement("Modifier");
	while (pModifier)
	{
		if (!pModifier->Attribute("name"))
			return false;
		Stats::StatName statName = Stats::statMap[pModifier->Attribute("name")];
		double amount;
		if (pModifier->QueryDoubleAttribute("effect", &amount) != tinyxml2::XMLError::XML_SUCCESS)
			return false;
		m_modifiers[statName] = amount;
		pModifier = pModifier->NextSiblingElement("Modifier");
	}
	return true;
}

const char* ItemComponent::COMPONENT_NAME = "ItemComponent";
const char* BaseItemComponent::COMPONENT_NAME = "BaseItemComponent";
const char* ItemRenderComponent::COMPONENT_NAME = "ItemRenderComponent";
const char* RestorationItemComponent::COMPONENT_NAME = "RestorationItemComponent";
const char* ConsumableItemComponent::COMPONENT_NAME = "ConsumableItemComponent";
const char* EquipableItemComponent::COMPONENT_NAME = "EquipableItemComponent";