#include "ComponentBase.h"

ComponentBase::ComponentBase() : m_owner(0)
{
}

void ComponentBase::PostInit()
{

}

void ComponentBase::SetOwner(StrongEntityPtr owner)
{
	m_owner = owner;
}

WeakEntityPtr ComponentBase::GetOwner() const
{
	return WeakEntityPtr(m_owner);
}
