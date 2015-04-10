#include "EntityList.h"

bool EntityList::LoadEntities(string filename)
{

	if (m_entityXMLDocument.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	m_xmlFilepath = filename;
	tinyxml2::XMLNode* pBase = m_entityXMLDocument.FirstChild();
	tinyxml2::XMLElement* pEntity = pBase->FirstChildElement("Entity");
	while (pEntity)
	{
		Entity newEntity;
		newEntity.xmlElement = pEntity;
		newEntity.name = pEntity->Attribute("name");
		if (ParseEntity(pEntity, newEntity))
			m_entities[reinterpret_cast<int>(HashedString::hash_name(newEntity.name.c_str()))] = newEntity;
		pEntity = pEntity->NextSiblingElement("Entity");
	}
}

bool EntityList::ParseEntity(XMLElement* pEntity, Entity& newEntity)
{
	const tinyxml2::XMLElement* pEntityComp = pEntity->FirstChildElement("Component");
	while (pEntityComp)
	{
		if (!pEntityComp->Attribute("name"))
			return false;
		string name = pEntityComp->Attribute("name");

		Component newComp;
		if (!m_componentList->GetComponentByID(reinterpret_cast<int>(HashedString::hash_name(name.c_str())), newComp))
			return false;
		for (int i = 0; i < newComp.fields.size(); ++i)
		{
			if (!pEntityComp->Attribute(newComp.fields[i].name.c_str()))
				break;
			newComp.fields[i].value = pEntityComp->Attribute(newComp.fields[i].name.c_str());

		}

		if (newComp.lists.size() > 0)
		{
			List compListDef = newComp.lists[0];
			List newList;
			newList.name = compListDef.name;
			newList.entryName = compListDef.entryName;
			const tinyxml2::XMLElement* pEntityList = pEntityComp->FirstChildElement(compListDef.name.c_str());
			const tinyxml2::XMLElement* pEntityListEntry = pEntityList->FirstChildElement(compListDef.entryName.c_str());
			while (pEntityListEntry)
			{
				ListEntry entityList;
				for (int i = 0; i < compListDef.entries[0].fields.size(); ++i)
				{
					Field newField;
					newField.name = compListDef.entries[0].fields[i].name;
					newField.valueType = compListDef.entries[0].fields[i].valueType;
					if (!pEntityListEntry->Attribute(newField.name.c_str()))
						break;
					newField.value = pEntityListEntry->Attribute(newField.name.c_str());
					entityList.fields.push_back(newField);
				}
				newList.entries.push_back(entityList);
				pEntityListEntry = pEntityListEntry->NextSiblingElement(newList.entryName.c_str());
			}
			newComp.lists[0] = newList;
		}

		newEntity.components.push_back(newComp);
		pEntityComp = pEntityComp->NextSiblingElement("Component");
	}

	return true;
}

void EntityList::GetEntities(EntityMap& map)
{
	map = m_entities;
}

bool EntityList::GetEntityByID(int id, Entity& item)
{
	EntityMap::iterator it = m_entities.find(id);
	if (it == m_entities.end())
		return false;
	item = it->second;
	return true;
}

void EntityList::SetEntityByID(int id, Entity item)
{
	EntityMap::iterator it = m_entities.find(id);

	if (it != m_entities.end())
	{
		item.xmlElement = it->second.xmlElement;
		it->second = item;
		EditEntryInXML(item);
	}
	else
	{
		m_entities[id] = item;
		item.xmlElement = m_entityXMLDocument.NewElement("Entity");
		EditEntryInXML(item);
		auto pEntityCatalog = m_entityXMLDocument.FirstChild();
		pEntityCatalog->InsertEndChild(item.xmlElement);
	}

	//Remove and reinsert item in case of name/ID changed
	m_entities.erase(it);
	m_entities[reinterpret_cast<int>(HashedString::hash_name(item.name.c_str()))] = item;

	m_entityXMLDocument.SaveFile(m_xmlFilepath.c_str());
}


void EntityList::EditEntryInXML(Entity& item)
{
	item.xmlElement->DeleteChildren();
	item.xmlElement->SetAttribute("name", item.name.c_str());
	Component currComp;
	for (int i = 0; i < item.components.size(); ++i)
	{
		XMLElement* newComponent = m_entityXMLDocument.NewElement("Component");
		currComp = item.components[i];
		newComponent->SetAttribute("name", currComp.name.c_str());
		for (int j = 0; j < currComp.fields.size(); ++j)
		{
			newComponent->SetAttribute(currComp.fields[j].name.c_str(), currComp.fields[j].value.c_str());
		}

		for (int j = 0; j < currComp.lists.size(); ++j)
		{
			List currList = currComp.lists[j];
			XMLElement* newList = m_entityXMLDocument.NewElement(currList.name.c_str());
			for (int k = 0; k < currList.entries.size(); ++k)
			{
				ListEntry currListEntry = currList.entries[k];
				XMLElement* newListEntry = m_entityXMLDocument.NewElement(currList.entryName.c_str());
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