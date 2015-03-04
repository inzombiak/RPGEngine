#ifndef ENTITY_H
#define ENTITY_H

#include <unordered_map>
#include "custom_defininitions.h"

using std::unordered_map;

class Entity
{

public:
	Entity() {};
	
	void Init(EntityID id, EntityType type)
	{
		m_ID = id;
		m_type = type;
	}

	inline EntityType GetType() const
	{
		return m_type;
	}
	inline EntityID GetID() const
	{
		return m_ID;
	}

	WeakComponentPtr GetComponent(ComponentID compType);
	bool AddComponent(StrongComponentPtr component);

	void PostInit();

private:
	EntityID m_ID;
	EntityType m_type;
	typedef unordered_map<ComponentID, StrongComponentPtr, std::hash<int>> ComponentMap;
	ComponentMap m_components;
};

#endif