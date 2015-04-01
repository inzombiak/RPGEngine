#pragma once

#include "tinyxml2.h"
#include "custom_defininitions.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

using tinyxml2::XMLElement;

/*
Defines XML attribute
*/
struct Field
{
	string name;
	string valueType;
};
/*
Used for defining components that have lists, i.e. InventoryComponent or EquipmentComponent
*/

struct List
{
	string name; //Name of the list
	string entryName; // Name of entries in the list
	vector<Field> fields; //Entries fields;
};

struct Component
{
	string name;
	bool isItemComponent;
	XMLElement* xmlElement;
	vector<Field> fields;
	vector<List> lists;
};

