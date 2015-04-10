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

struct Entity
{
	string name;
	XMLElement* xmlElement;
	vector<Component> components;
};

typedef  std::map< int, Entity > EntityMap;

class EntityList
{
public:
	bool LoadEntities(string filename);
	void GetEntities(EntityMap&);
	bool GetEntityByID(int id, Entity& item);
	void SetEntityByID(int id, Entity item);
	void SetComponentList(ComponentList* cl)
	{
		m_componentList = cl;
	}
private:
	bool ParseEntity(XMLElement*, Entity&);
	void EditEntryInXML(Entity&);

	string m_xmlFilepath;
	tinyxml2::XMLDocument m_entityXMLDocument;
	ComponentList* m_componentList;
	EntityMap m_entities;
};

