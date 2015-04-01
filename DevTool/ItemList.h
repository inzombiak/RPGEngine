#pragma once
#include "custom_defininitions.h"
#include "Component.h"

struct Item
{
	string name;
	vector<Component> components;
};

class XMLElement;
class ItemList
{
public:
	bool LoadItems(string filename);
private:
	bool ParseComponent(XMLElement* pComp, Component& newComp);
	vector<Item> m_itemList;
};

