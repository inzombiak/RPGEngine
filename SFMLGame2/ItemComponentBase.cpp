#include "ItemComponentBase.h"

bool BaseItemComponent::Init(const XMLElement* componentNode)
{
	if (componentNode->Attribute("name"))
	{
		m_name = componentNode->Attribute("name");
		return true;
	}
	return false;
	
}