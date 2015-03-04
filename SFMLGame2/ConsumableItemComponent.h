#ifndef CONSUMABLE_ITEM_COMPONENT_H
#define CONSUMABLE_ITEM_COMPONENT_H

#include "ItemComponentBase.h"

class ConsumableItemComponent : public ItemComponentBase
{
public:

	virtual bool Init(const XMLElement* componentNode) override;
	virtual void PostInit() override;
	void Update(float dt);

	int GetUsesRemaining()
	{
		return m_uses;
	}
	void SetUses(int uses)
	{
		m_uses = uses;
	}
	int GetMaxStackSize()
	{
		return m_maxStack;
	}

private:
	int m_uses;
	int m_maxStack;
};

#endif
