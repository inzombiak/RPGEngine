#include "ItemList.h"


bool ItemList::LoadItems(string filename)
{

	if (m_itemXMLDocument.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	m_xmlFilepath = filename;
	tinyxml2::XMLNode* pBase = m_itemXMLDocument.FirstChild();
	tinyxml2::XMLElement* pItem = pBase->FirstChildElement("Item");
	while (pItem)
	{
		Item newItem;
		newItem.xmlElement = pItem;
		newItem.name = pItem->Attribute("name");
		if (ParseItem(pItem, newItem))
			m_items[reinterpret_cast<int>(HashedString::hash_name(newItem.name.c_str()))] = newItem;
		pItem = pItem->NextSiblingElement("Item");
	}
}

bool ItemList::ParseItem(XMLElement* pItem, Item& newItem)
{
	const tinyxml2::XMLElement* pItemComp = pItem->FirstChildElement("Component");
	newItem.slot = Slots::SlotName::NaN;
	while (pItemComp)
	{
		if (!pItemComp->Attribute("name"))
			return false;
		string name = pItemComp->Attribute("name");

		Component newComp;
		if (!m_componentList->GetComponentByID(reinterpret_cast<int>(HashedString::hash_name(name.c_str())), newComp))
			return false;
		for (int i = 0; i < newComp.fields.size(); ++i)
		{
			if (!pItemComp->Attribute(newComp.fields[i].name.c_str()))
				break;
			newComp.fields[i].value = pItemComp->Attribute(newComp.fields[i].name.c_str());
			
		}

		/*
		Temporary
		*/
		if (newComp.name.compare("EquipableItemComponent") == 0)
		{
			newItem.slot = Slots::slotStringtoEnumMap.find(pItemComp->Attribute("slot"))->second;
		}

		if (newComp.lists.size() > 0)
		{
			List compListDef = newComp.lists[0];
			List newList;
			newList.name = compListDef.name;
			newList.entryName = compListDef.entryName;
			const tinyxml2::XMLElement* pItemList = pItemComp->FirstChildElement(compListDef.name.c_str());
			const tinyxml2::XMLElement* pItemListEntry = pItemList->FirstChildElement(compListDef.entryName.c_str());
			while (pItemListEntry)
			{
				ListEntry itemList;
				for (int i = 0; i < compListDef.entries[0].fields.size(); ++i)
				{
					Field newField;
					newField.name = compListDef.entries[0].fields[i].name;
					newField.valueType = compListDef.entries[0].fields[i].valueType;
					if (!pItemListEntry->Attribute(newField.name.c_str()))
						break;
					newField.value = pItemListEntry->Attribute(newField.name.c_str());
					itemList.fields.push_back(newField);
				}
				newList.entries.push_back(itemList);
				pItemListEntry = pItemListEntry->NextSiblingElement(newList.entryName.c_str());
			}
			newComp.lists[0] = newList;
		}
	
		newItem.components.push_back(newComp);
		pItemComp = pItemComp->NextSiblingElement("Component");
	}

	return true;
}

void ItemList::GetItems(ItemMap& map)
{
	map = m_items;
}

bool ItemList::GetItemByID(int id, Item& item)
{
	ItemMap::iterator it = m_items.find(id);
	if (it == m_items.end())
		return false;
	item = it->second;
	return true;
}

void ItemList::SetItemByID(int id, Item item)
{
	ItemMap::iterator it = m_items.find(id);
	if (it != m_items.end())
	{
		item.xmlElement = it->second.xmlElement;
		it->second = item;
		EditEntryInXML(item);
	}
	else
	{
		m_items[id] = item;
		item.xmlElement = m_itemXMLDocument.NewElement("Item");
		EditEntryInXML(item);
		auto pItemCatalog = m_itemXMLDocument.FirstChild();
		pItemCatalog->InsertEndChild(item.xmlElement);
	}
	m_itemXMLDocument.SaveFile(m_xmlFilepath.c_str());
}


void ItemList::EditEntryInXML(Item& item)
{
	item.xmlElement->DeleteChildren();
	item.xmlElement->SetAttribute("name", item.name.c_str());
	Component currComp;
	for (int i = 0; i < item.components.size(); ++i)
	{
		XMLElement* newComponent = m_itemXMLDocument.NewElement("Component");
		currComp = item.components[i];
		newComponent->SetAttribute("name", currComp.name.c_str());
		for (int j = 0; j < currComp.fields.size(); ++j)
		{
			newComponent->SetAttribute(currComp.fields[j].name.c_str(), currComp.fields[j].value.c_str());	
		}
		
		for (int j = 0; j < currComp.lists.size(); ++j)
		{
			List currList = currComp.lists[j];
			XMLElement* newList = m_itemXMLDocument.NewElement(currList.name.c_str());
			for (int k = 0; k < currList.entries.size(); ++k)
			{
				ListEntry currListEntry = currList.entries[k];
				XMLElement* newListEntry = m_itemXMLDocument.NewElement(currList.entryName.c_str());
				for (int p = 0; p < currListEntry.fields.size(); ++p)
				{
					newListEntry->SetAttribute(currListEntry.fields[p].name.c_str(), currListEntry.fields[p].value.c_str());
				}
				newList->InsertEndChild(newListEntry);
			}
			newComponent->InsertEndChild(newList);
		}

		item.xmlElement->InsertEndChild(newComponent);
	}
}

void ItemList::GetItemsBySlot(std::map<Slots::SlotName, vector<string>>& target)
{
	for (auto it = Slots::slotEnumtoStringMap.begin(); it != Slots::slotEnumtoStringMap.end(); ++it)
		target[it->first] = vector < string >() ;

	for (auto it = m_items.begin(); it != m_items.end(); ++it)
		target[it->second.slot].push_back(it->second.name);
}