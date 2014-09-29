//#include "ItemCatalog.h"
//
//void ItemCatalog::Initialize(string itemFile)
//{
//	if (!m_initialized)
//		ReadItems(itemFile);
//}
//
//void ItemCatalog::ReadItems(string itemFile)
//{
//	ifstream f;
//	string garbage;
//	string name = "", description = "", icon = "";
//	char input = '/';
//
//	f.open(itemFile);
//
//	while (!f.eof())
//	{
//		//Read name of item
//		garbage = "";
//		while (!f.eof())
//		{
//			f.get(input);
//			garbage += input;
//			if (garbage == "Name: ")
//				break;
//		}
//
//		f.get(input);
//		name = "";
//		while (input != '\n')
//		{
//			name += input;
//			f.get(input);
//		}
//
//		//Read item description
//		garbage = "";
//		while (!f.eof())
//		{
//			f.get(input);
//			garbage += input;
//			if (garbage == "Description: ")
//				break;
//		}
//
//		f.get(input);
//		description = "";
//		while (input != '\n')
//		{
//			if (f.eof())
//				break;
//			description += input;
//			f.get(input);
//		}
//
//		//Read item icon
//		garbage = "";
//		while (!f.eof())
//		{
//			f.get(input);
//			garbage += input;
//			if (garbage == "Icon: ")
//				break;
//		}
//
//		f.get(input);
//		icon = "";
//		while (input != '\n')
//		{
//			if (f.eof())
//				break;
//			icon += input;
//			f.get(input);
//		}
//		CreateItem(name, description, icon);
//		getline(f,garbage);
//	}
//	
//	f.close();
//	m_initialized = true;
//}
//
//void ItemCatalog::CreateItem(string name, string description, string icon)
//{
//	icon = "data/Item Icons/" + icon;
//	m_itemCatalog.insert(std::make_pair(name, new Item(name, description, icon)));
//}
//
//void ItemCatalog::AddItem(Item& newItem)
//{
//	
//}
//
//bool ItemCatalog::GetItem(string itemName, Item& target)
//{
//	map<string, Item*>::iterator query;
//	query = m_itemCatalog.find(itemName);
//	if (query == m_itemCatalog.end())
//		return false;
//	else
//		target = *m_itemCatalog[itemName];
//	return true;
//}
//
//bool ItemCatalog::GetItemDescription(string itemName, string& target)
//{
//	map<string, Item*>::iterator query;
//	query = m_itemCatalog.find(itemName);
//	if (query == m_itemCatalog.end())
//		return false;
//	else
//		target = query->second->GetDescription();
//	return true;
//}
//
//void ItemCatalog::Destroy()
//{
//	for (map<string, Item*>::iterator i = m_itemCatalog.begin(); i != m_itemCatalog.end(); i++)
//	{
//		delete i->second;
//	}
//
//	delete m_instance;
//}
//
//bool ItemCatalog::m_initialized;
//map<string, Item*> ItemCatalog::m_itemCatalog;
//ItemCatalog* ItemCatalog::m_instance;