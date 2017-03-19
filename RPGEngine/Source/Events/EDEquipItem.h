#ifndef ED_EQUIP_ITEM_H
#define ED_EQUIP_ITEM_H

#include "IEventData.h"
#include "../Utilities/GameDefs.h"

class EDEquipItem : public IEventData
{
public:
	EDEquipItem(Equipment::SlotName slotName, unsigned int itemID, unsigned int componentID, bool toEquip)
	{
		m_slotName = slotName;
		m_itemID = itemID; 
		m_componentID = componentID;
		if (toEquip)
			m_eventType = EventDefs::EQUIP_ITEM;
		else
			m_eventType = EventDefs::UNEQUIP_ITEM;
	}

	Equipment::SlotName GetSlotName() const
	{
		return m_slotName;
	}

	unsigned int GetItemID() const
	{
		return m_itemID;
	}
	unsigned int GetComponentID() const
	{
		return m_componentID;
	}

private:
	//Slot of the item that is being equipped/uneqipped
	Equipment::SlotName m_slotName;
	//ID of the item
	unsigned int m_itemID;
	//ID of component
	unsigned int m_componentID;
};

#endif