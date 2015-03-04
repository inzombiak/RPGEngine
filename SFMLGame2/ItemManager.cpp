#include "ItemManager.h"

#include "tinyxml2.h"

using tinyxml2::XMLElement;

void ItemManager::LoadItemCatalog(string filepath)
{

	/*
	Error handling
	*/
	tinyxml2::XMLDocument file;
	file.LoadFile(filepath.c_str());

	tinyxml2::XMLNode* pRoot = file.FirstChild();
	XMLElement* pItem = pRoot->FirstChildElement("Item");
	Item newItem;
	while (pItem != nullptr)
	{
		if (InitializeItem(pItem, newItem))
			m_itemCatalog.insert(std::pair<string, Item>(pItem->Attribute("name"), newItem));
		pItem = pRoot->NextSiblingElement("Item");
	}
}

bool ItemManager::InitializeItem(XMLElement* node, Item& item)
{
	if (!node->Attribute("name"))
		return false;
	item.name = node->Attribute("name");

	if (!node->Attribute("description"))
		return false;
	item.description = node->Attribute("description");

	if (!node->Attribute("iconfilepath"))
		return false;
	item.iconFilepath = node->Attribute("description");
	
	XMLElement* pItemComponent = node->FirstChildElement("ItemComponent");
	StrongItemComponentPtr newItemComponent;
	while (pItemComponent)
	{
		newItemComponent = CreateItemComponent(node);
		if (!newItemComponent)
			return false;
		else
			item.itemComponents.push_back(newItemComponent);
		pItemComponent = node->NextSiblingElement("ItemComponent");
	}
	return true;
}
bool ItemManager::Register(string name, StrongItemComponentPtr(*CreationFunction)(XMLElement*))
{
	auto find = m_creationFunctions.find(name);
	if (find == m_creationFunctions.end())
	{
		m_creationFunctions[name] = CreationFunction;
		return true;
	}
	return false;
}

StrongItemComponentPtr ItemManager::CreateItemComponent(XMLElement* node)
{
	StrongItemComponentPtr newComponent;

	if (node->Attribute("type"))
	{
		newComponent = m_creationFunctions.find(node->Attribute("type"))->second(node);
	}

	return newComponent;
}