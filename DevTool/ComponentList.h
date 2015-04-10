#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

#include "custom_defininitions.h"
#include "Component.h"
#include "SFGUI\SFGUI.hpp"
#include "tinyxml2.h"

#include <string>
#include <vector>
#include <map>

using std::vector;
using std::string;
using tinyxml2::XMLElement;

class ComponentList
{
public:
	bool LoadComponents(std::string);
	void GetComponents(ComponentMap&);
	bool GetComponentByID(int id, Component& component);
	void SetComponentByID(int id, Component component);

private:
	bool ParseComponent(XMLElement*, Component&);
	void AddEntryToXML(Component&);
	void EditEntryInXML(Component&);
	string m_xmlFilepath;
	tinyxml2::XMLDocument m_componentXMLDocument;
	ComponentMap m_components;
};

#endif

