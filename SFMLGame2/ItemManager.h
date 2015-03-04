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

using std::map;
using std::string;
using std::vector;

using tinyxml2::XMLElement;

class ItemComponent;
typedef std::shared_ptr<ItemComponent> StrongItemComponentPtr;

struct Item
{
	string name;
	string description;
	string iconFilepath;
	vector<StrongItemComponentPtr> itemComponents;
};

class ItemManager
{
public:
	void LoadItemCatalog(string filepath);
	StrongComponentPtr CreateItemComponent(string itemname);

private:
	bool InitializeItem(XMLElement* node, Item& item);
	StrongItemComponentPtr CreateItemComponent(XMLElement* node);

	typedef map<string, Item> ItemCatalog;
	ItemCatalog m_itemCatalog;

	typedef StrongItemComponentPtr (*ItemComponentCreationFunc)(XMLElement*);
	typedef map<string, ItemComponentCreationFunc> ItemComponentCreationFunctions;
	ItemComponentCreationFunctions m_creationFunctions;
	bool Register(string name, StrongItemComponentPtr(*CreationFunction)(XMLElement*));
};


#endif