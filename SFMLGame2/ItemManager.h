#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

/*
TODO
Merge Item and ItemComponent creation with Entity nad EntityComponent creation
*/

#include <string>
#include <map>
#include <vector>
#include "custom_defininitions.h"
#include "tinyxml2.h"

#include "ItemComponent.h"

using std::map;
using std::string;
using std::vector;

using tinyxml2::XMLElement;

class ItemManager
{
public:
	ItemManager();
	void LoadItemCatalog(string filepath);
	static StrongComponentPtr CreateInventoryComponent();
	static StrongComponentPtr CreateItemPickupComponent();
	static bool CreateItemByID(ItemID itemID, Item& item);

private:
	//Stores information for an item, not actual item
	class ItemDefinition
	{
	public:
		vector<StrongItemComponentPtr> itemComponents;
		XMLElement* renderComponenetInfo;
	};

	tinyxml2::XMLDocument m_xmlFile;
	bool InitializeItem(XMLElement* node, ItemDefinition& item);
	StrongItemComponentPtr CreateItemComponent(XMLElement* node);
	
	typedef map<ItemID, ItemDefinition> ItemCatalog;
	static ItemCatalog m_itemCatalog;
};

template<class SubClass>
StrongItemComponentPtr ItemComponentCreationFunction(void)
{
	return StrongItemComponentPtr(new SubClass);
}
#endif