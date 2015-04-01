#pragma once
#include "ComponentBase.h"
#include "ItemComponent.h"

//TODO Add enum of possible responces, I.E NO_SLOT,NO_ITEM etc.?

class EquipableItemComponent;
class EquipmentComponent : public ComponentBase
{
public:
	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override;
	void Update(float dt) {};
	static const char* COMPONENT_NAME;
	const char* GetName()
	{
		return COMPONENT_NAME;
	}
	//Returns true if slot exists otherwise returns false;
	bool HasSlot(Equipment::SlotName slot);
	/*
	Returns true if slot exists and an item was equiped and places it in the provided arguement, otherwise returns false if slot doesn't exist or no item was equipped
	*/
	bool Equip(Equipment::SlotName slot, Item item, Item& prevItem);
	/*
	Recommend using HasSlot before calling
	Returns true if slot exists and an item was equiped and places ID of item in the provided arguement, otherwise returns false if slot does not exist
	*/
	bool Unequip(Equipment::SlotName slot, Item& item);

	//Returns equipment. Temporary until I move inventory management to playerobserver
	const std::map<Equipment::SlotName, Item>& GetEquipment()
	{
		return m_equipment;
	}
private:

	//Called after a new item is equipped
	void ApplyModifiers(Item& newItem);
	//Called when an item is unequipped
	void RemoveModifiers(Item& oldItem);
	std::map<Equipment::SlotName, Item> m_equipment;
	std::map<Equipment::SlotName, bool> m_availableSlots;
};

