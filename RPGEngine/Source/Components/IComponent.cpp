#include "IComponent.h"

IComponent::IComponent() : m_owner(0), m_inUse(true)
{
}

void IComponent::PostInit()
{

}

void IComponent::SetOwner(StrongEntityPtr owner)
{
	m_owner = owner;
}

WeakEntityPtr IComponent::GetOwner() const
{
	return WeakEntityPtr(m_owner);
}
