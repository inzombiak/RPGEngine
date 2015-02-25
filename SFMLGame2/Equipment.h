#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "Item.h"

class Equipment : public Item 
{
public:
	
	void Use()
	{
		m_equipped = !m_equipped;
	};

private:
	std::string m_slot;
	bool m_equipped;
	int m_power;
};

#endif
