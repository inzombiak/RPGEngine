#ifndef INVENTORYCOMPONENT_H
#define INVENTORYCOMPONENT_H

#include "ComponentBase.h"
#include "Item.h"

#include <vector>

using std::vector;

class InventoryComponent : public ComponentBase
{

public:
	
private:
	vector < int > m_vec ;
};

#endif
