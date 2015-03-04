#ifndef ItemComponentBase_H
#define ItemComponentBase_H

/*
All Items have an instance of this component.
This holds basic information for the item as a map of ItemComponents.
*/
#include <map>
#include <memory>

#include "tinyxml2.h"

using std::map;

using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

typedef int ItemComponentID;
typedef std::shared_ptr<ItemComponentBase> StrongItemComponentPtr;
typedef std::weak_ptr<ItemComponentBase> WeakItemComponentPtr;

class ItemComponentBase
{
public:
	ItemComponentBase();
	virtual ~ItemComponentBase() {};

	virtual bool Init(const XMLElement* componentNode) = 0;
	virtual void PostInit() = 0;

	virtual void Update(float dt) {};

	const char* GetName()
	{
		return COMPONENT_NAME;
	}

	void SetQuantity(int newQuantity)
	{
		m_quantity = newQuantity;
	}

	int GetQuantity() const
	{
		return m_quantity;
	}

	void SetName(std::string name)
	{
		m_name = name;
	}

	std::string GetName() const
	{
		return m_name;
	}

private:
	static const char* COMPONENT_NAME;

	typedef map<ItemComponentID, StrongItemComponentPtr> ItemComponentMap;
	ItemComponentMap m_components;

	std::string m_name;
	std::string m_description;
	int m_quantity;
};

#endif