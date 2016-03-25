#ifndef ENTITY_H
#define ENTITY_H

#include <unordered_map>
#include "Utilities\GameDefs.h"

using std::unordered_map;
/*
Entities are main game objects. Their properties are defined based on the components attached to them
*/

using std::map;
using std::weak_ptr;
class Entity
{

public:
	Entity() {};
	
	void Init(int id, EntityNameID type)
	{
		m_ID = id;
		m_nameID = type;
	}

	EntityNameID  GetNameID() const
	{
		return m_nameID;
	}
	int GetID() const
	{
		return m_ID;
	}

	void SetInUse(bool inUse);
	bool GetInUse()
	{
		return m_inUse;
	}
	WeakComponentPtr GetComponent(ComponentID compType);
	bool AddComponent(StrongComponentPtr component);

	void PostInit();

private:
	int m_ID; //Unique to each entity
	EntityNameID m_nameID; //Unique to each class of entity, used for referencing definition in the XML
	bool m_inUse;
	typedef unordered_map<ComponentID, StrongComponentPtr, std::hash<int>> ComponentMap;
	ComponentMap m_components;

	template <class Component>
	weak_ptr<Component> GetActorComponentByID(ComponentID componentID)
	{
		//Check if enitiy contains the component
		EntityComponents::iterator componentIterator = m_components.find(componentID);
		if (componentIterator != m_components.end())
		{
			//Get the component, cast it to the derived type, store it in weak pointer and return
			StrongEntityComponentPtr pBase(componentIterator->second);
			weak_ptr<Component> pDeriv(static_pointer_cast(pBase));
			return pDeriv;
		}
		else
		{
			//Return empty pointer if no such component is found
			return weak_ptr<Component>();
		}
	}
};

#endif