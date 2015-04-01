#include "ItemList.h"
#include "tinyxml2.h"



bool ItemList::LoadItems(string filename)
{
	//tinyxml2::XMLDocument doc;
	//if (doc.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	//	return false;

	//tinyxml2::XMLNode* pBase = doc.FirstChild();
	//XMLElement* pItem = pBase->FirstChildElement();
	//XMLElement* pComp;
	//Item newItem;
	//while (pItem)
	//{
	//	if (pItem->Attribute("name"))
	//	{
	//		newItem.name = pItem->Attribute("name");
	//		pComp = pItem->FirstChildElement("Component");
	//		while (pComp)
	//		{

	//		}
	//	}
	//}

	return true;
}

bool ItemList::ParseComponent(XMLElement* pComp, Component& newComp)
{
	/*const tinyxml2::XMLAttribute* pCompAttrib = pComp->FirstAttribute();
	Field newField;
	while (pCompAttrib)
	{
		
	}
	return true;*/
	return true;
}
