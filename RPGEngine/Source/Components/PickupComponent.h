#include "IComponent.h"

class IPickupComponent : public IComponent
{
public:
	virtual void Apply(StrongEntityPtr target) = 0;
};

class ItemPickupComponent : public IPickupComponent
{
public:
	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override ;
	void Update(float dt) {};
	virtual	void Apply(StrongEntityPtr target) override;

	static const char* COMPONENT_NAME;
	const char* GetName()
	{
		return COMPONENT_NAME;
	}
	
	std::string GetItemName()
	{
		return m_itemName;
	}
	void SetItemName(std::string newName)
	{
		m_itemName = newName;
	}

	int GetQuantity()
	{
		return m_quantity;
	}
	void SetQuantity(int newQuantity)
	{
		m_quantity = newQuantity;
	}

private:
	std::string m_itemName; //Name of item
	int m_quantity; //Amount to be added on pickup
	bool m_persistent; //If this is true, object will not be destroyed after being applied
};