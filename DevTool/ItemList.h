#pragma once
#include "ComponentList.h"
#include "custom_defininitions.h"
#include "Component.h"
#include "tinyxml2.h"

#include <string>
#include <vector>
#include <map>

using std::vector;
using std::string;
using tinyxml2::XMLElement;

struct Item
{
	string name;
	XMLElement* xmlElement;
	vector<Component> components;
	Slots::SlotName slot;
};

typedef  std::map< int, Item > ItemMap;

class ItemList
{
public:
	bool LoadItems(string filename);
	void GetItems(ItemMap&);
	bool GetItemByID(int id, Item& item);
	void SetItemByID(int id, Item item);
	void SetComponentList(ComponentList* cl)
	{
		m_componentList = cl;
	}
	void GetItemsBySlot(std::map<Slots::SlotName, vector<string>>&);
private:
	bool ParseItem(XMLElement*, Item&);
	void EditEntryInXML(Item&);

	string m_xmlFilepath;
	tinyxml2::XMLDocument m_itemXMLDocument;
	ComponentList* m_componentList;
	ItemMap m_items;
};

