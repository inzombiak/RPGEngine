#ifndef CONSUMABLEITEM_H
#define CONSUMABLEITEM_H

#include "Item.h"

class ConsumableItem : public Item
{
public:
	
	ConsumableItem(int effectAmount, void(*useFunction) (int))
	{
		m_effectAmount = effectAmount;
		m_useFunction = useFunction;
	}

	void Use()
	{
		m_useFunction(m_effectAmount);
	}

private:
	int m_effectAmount;
	void (*m_useFunction) (int);
};

#endif