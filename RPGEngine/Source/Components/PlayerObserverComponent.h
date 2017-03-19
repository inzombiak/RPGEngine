#ifndef UI_OBSERVER_COMPONENT_H
#define UI_OBSERVER_COMPONENT_H

/*
Acts as a liason between the Player and the UI
Currently, only one instance of this component must exist and it must be tied to the Player
Contains flags for player related information i.e. dead, inventory open etc.
*/
#include "IComponent.h"

class UIManager;
class PlayerObserverComponent : public IComponent
{
public:

	virtual bool Init(XMLElement* node) override { return true; };
	virtual void PostInit() override{};
	virtual void Update(float dt) {};
	static const char* COMPONENT_NAME;
	const char* GetName()
	{
		return COMPONENT_NAME;
	}

	void ToggleInventory()
	{
		m_inventoryOpen = !m_inventoryOpen;
	}
	void ToggleEquipment()
	{
		m_equipmentOpen = !m_equipmentOpen;
	}

	bool InventoryOpen()
	{
		return m_inventoryOpen;
	}
	bool EquipmentOpen()
	{
		return m_equipmentOpen;
	}
private:
	bool m_inventoryOpen = false;
	bool m_equipmentOpen = false;
};

#endif 