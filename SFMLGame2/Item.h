#ifndef ITEM_H
#define ITEM_H

#include "ComponentBase.h"

class Item : public ComponentBase
{
public:
		
	virtual void Use() = 0;

	void SetQuantity(int newQuantity)
	{
		m_quantity = newQuantity;
	}

	int GetQuantity() const
	{
		return m_quantity;
	}

	void SetName(std::string name)
	{
		m_name = name;
	}

	std::string GetName() const
	{
		return m_name;
	}

private:
	std::string m_name;
	int m_quantity;
};

#endif