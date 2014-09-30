#ifndef ENTITY_H
#define ENTITY_H

/*
Entities are main game objects. Their properties are defined based on the components attached to them
*/


#include <map>

#include "interfaces.h"

using std::map;
using std::weak_ptr;

class Entity
{

public:



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

private:
	typedef map<ComponentID, StrongEntityComponentPtr> EntityComponents;  //typedef because simplicity. Components are stored based on their unique ID
	
	EntityComponents m_components; //Container for components
	EntityID m_ID; //Enities ID

	void AddComponent(StrongEntityComponentPtr pComponent) //Add a component, meant to be used by EntityFactory
};

#endif