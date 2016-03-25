#ifndef INVENTORY_COMPONENT_H
#define INVENTORY_COMPONENT_H

#include "IComponent.h"
#include "ItemComponent.h"

#include <vector>

using std::vector;

class InventoryComponent : public IComponent
{
public:
	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override {};
	void Update(float dt) {};

	const char* GetName()
	{
		return COMPONENT_NAME;
	}
	
	bool AddItem(string itemName, int quantity);
	bool AddItem(Item newItem, int quantity);
	bool AddItem(ItemID newItem, int quantity);
	const std::map<ItemID, Item>& GetItems();
	//Setter and Getters

	void SetDroppable(bool droppable)
	{
		m_canDrop = droppable;
	}
	bool GetDroppable()
	{
		return m_canDrop;
	}

	void SetMaxSize(int maxSize)
	{
		m_maxSize = maxSize;
	}
	int GetMaxSize()
	{
		return m_maxSize;
	}

	bool IsFull()
	{
		return m_heldItems.size() == m_maxSize;
	}
	void UseItem(ItemID);
	void DropItem(ItemID);
	void DeleteItem(ItemID);
	void ToggleDisplay();

	const static char* COMPONENT_NAME;
private:
	std::map<ItemID, Item> m_heldItems;
	bool m_canDrop; //Can drop items when drop criteria(?) is met
	int m_maxSize;
};

#endif
