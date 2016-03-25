#ifndef ED_EQUIP_ITEM_H
#define ED_EQUIP_ITEM_H

#include "IEventData.h"
#include "../Utilities/GameDefs.h"

class EDEquipItem : public IEventData
{
public:
	EDEquipItem(Equipment::SlotName slotName, unsigned int itemID, bool toEquip) : m_slotName(slotName), m_itemID(itemID)
	{
		if (toEquip)
			m_eventType = EventDefs::EQUIP_ITEM;
		else
			m_eventType = EventDefs::UNEQUIP_ITEM;
	}

	void 

private:
	//Slot of the item that is being equipped/uneqipped
	Equipment::SlotName m_slotName;
	//ID of the item
	unsigned int m_itemID;

};

#endif