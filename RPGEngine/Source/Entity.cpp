#include "Entity.h"
#include "Components\IComponent.h"

void Entity::PostInit()
{
	for (ComponentMap::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->PostInit();
	}
}

void Entity::SetInUse(bool inUse)
{
	m_inUse = inUse;
	for (ComponentMap::iterator it = m_components.begin(); it != m_components.end(); ++it)
		it->second->SetInUse(inUse);
}

WeakComponentPtr Entity::GetComponent(ComponentID compType)
{
	ComponentMap::iterator findComp = m_components.find(compType);

	if (findComp != m_components.end())
	{
		return WeakComponentPtr(findComp->second);
	}
	else
	{
		return WeakComponentPtr();
	}
}

bool Entity::AddComponent(StrongComponentPtr component)
{
	ComponentID compID = component->GetID();

	ComponentMap::iterator findComp = m_components.find(compID);

	if (findComp == m_components.end())
	{
		m_components.insert(std::pair<ComponentID, StrongComponentPtr>(compID, component));

		return true;
	}
	else
	{
		return false;
	}
}