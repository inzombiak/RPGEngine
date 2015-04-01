#include "ComponentList.h"


bool ComponentList::LoadComponents(string filename)
{
	if (m_componentXMLDocument.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	m_xmlFilepath = "testComponent.xml";
	tinyxml2::XMLNode* pBase = m_componentXMLDocument.FirstChild();
	tinyxml2::XMLElement* pEntityComponents = pBase->FirstChildElement("Entities");
	tinyxml2::XMLElement* pComp = pEntityComponents->FirstChildElement("Component");
	
	while (pComp)
	{
		Component newComp;
		newComp.xmlElement = pComp;
		if (ParseComponent(pComp, newComp))
			m_components[reinterpret_cast<int>(HashedString::hash_name(newComp.name.c_str()))] = newComp;
		newComp.isItemComponent = false;
		pComp = pComp->NextSiblingElement("Component");
	}
	tinyxml2::XMLElement* pItemComponents = pBase->FirstChildElement("Items");
	pComp = pItemComponents->FirstChildElement("Component");
	while (pComp)
	{
		Component newComp;
		newComp.xmlElement = pComp;
		if (ParseComponent(pComp, newComp))
			m_components[reinterpret_cast<int>(HashedString::hash_name(newComp.name.c_str()))] = newComp;
		newComp.isItemComponent = true;
		pComp = pComp->NextSiblingElement("Component");
	}
}

bool ComponentList::ParseComponent(XMLElement* pComp, Component& newComp)
{
	XMLElement* pField;
	XMLElement* pList;
	if (!pComp->Attribute("name"))
		return false;
	newComp.name = pComp->Attribute("name");

	pField = pComp->FirstChildElement("Field");
	while (pField)
	{
		Field newField;
		if (pField->Attribute("name") && pField->Attribute("type"))
		{
			newField.name = pField->Attribute("name");
			newField.valueType = pField->Attribute("type");
			newComp.fields.push_back(newField);
		}
		pField = pField->NextSiblingElement("Field");
	}

	pList = pComp->FirstChildElement("List");
	while (pList)
	{
		if (pList->Attribute("name") && pList->Attribute("entryname"))
		{
			List newList;
			newList.name = pList->Attribute("name");
			newList.entryName = pList->Attribute("entryname");
			pField = pList->FirstChildElement("Field");
			while (pField)
			{
				Field newField;
				if (pField->Attribute("name") && pField->Attribute("type"))
				{
					newField.name = pField->Attribute("name");
					newField.valueType = pField->Attribute("type");
					newList.fields.push_back(newField);
				}

				pField = pField->NextSiblingElement("Field");
			}
			newComp.lists.push_back(newList);
		}
		pList = pList->NextSiblingElement("List");
	}
	return true;
}

void ComponentList::GetComponents(ComponentMap& components)
{
	components = m_components;
}

bool ComponentList::GetComponentByID(int id, Component& component)
{
	ComponentMap::iterator it = m_components.find(id);
	if (it == m_components.end())
		return false;
	component = it->second;
	return true;
}

void ComponentList::SetComponentByID(int id, Component component)
{
	ComponentMap::iterator it = m_components.find(id);
	if (it != m_components.end())
	{
		component.xmlElement = it->second.xmlElement;
		it->second = component;
		EditEntryInXML(component);
	}
	else
	{
		m_components[id] = component;
		AddEntryToXML(component);
	}
}

void ComponentList::AddEntryToXML(Component& component)
{
	component.xmlElement = m_componentXMLDocument.NewElement("Component");
	component.xmlElement->SetAttribute("name", component.name.c_str());
	for (int i = 0; i < component.fields.size(); ++i)
	{
		XMLElement* newField = m_componentXMLDocument.NewElement("Field");
		newField->SetAttribute("name", component.fields[i].name.c_str());
		newField->SetAttribute("type", component.fields[i].valueType.c_str());
		component.xmlElement->InsertEndChild(newField);
	}
	for (int i = 0; i < component.lists.size(); ++i)
	{
		XMLElement* newList = m_componentXMLDocument.NewElement("List");
		newList->SetAttribute("name", component.lists[i].name.c_str());
		newList->SetAttribute("entryname", component.lists[i].entryName.c_str());
		for (int j = 0; j < component.lists[i].fields.size(); ++j)
		{
			XMLElement* newField = m_componentXMLDocument.NewElement("Field");
			newField->SetAttribute("name", component.lists[i].fields[j].name.c_str());
			newField->SetAttribute("type", component.lists[i].fields[j].valueType.c_str());
			component.xmlElement->InsertEndChild(newField);
		}
		component.xmlElement->InsertEndChild(newList);
	}
	tinyxml2::XMLNode* pBase = m_componentXMLDocument.FirstChild();
	tinyxml2::XMLElement* pEntityComponents;
	if (!component.isItemComponent)
		pEntityComponents = pBase->FirstChildElement("Entities");
	else
		pEntityComponents = pBase->FirstChildElement("Items");
	pEntityComponents->InsertEndChild(component.xmlElement);
	m_componentXMLDocument.SaveFile(m_xmlFilepath.c_str());
}

void ComponentList::EditEntryInXML(Component& component)
{
	component.xmlElement->DeleteChildren();
	component.xmlElement->SetAttribute("name", component.name.c_str());
	for (int i = 0; i < component.fields.size(); ++i)
	{
		XMLElement* newField = m_componentXMLDocument.NewElement("Field");
		newField->SetAttribute("name", component.fields[i].name.c_str());
		newField->SetAttribute("type", component.fields[i].valueType.c_str());
		component.xmlElement->InsertEndChild(newField);
	}
	for (int i = 0; i < component.lists.size(); ++i)
	{
		XMLElement* newList = m_componentXMLDocument.NewElement("List");
		newList->SetAttribute("name", component.lists[i].name.c_str());
		newList->SetAttribute("entryname", component.lists[i].entryName.c_str());
		for (int j = 0; j < component.lists[i].fields.size(); ++j)
		{
			XMLElement* newField = m_componentXMLDocument.NewElement("Field");
			newField->SetAttribute("name", component.lists[i].fields[j].name.c_str());
			newField->SetAttribute("type", component.lists[i].fields[j].valueType.c_str());
			component.xmlElement->InsertEndChild(newField);
		}
		component.xmlElement->InsertEndChild(newList);
	}
	m_componentXMLDocument.SaveFile(m_xmlFilepath.c_str());
}