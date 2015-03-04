#include "ConsumableItemComponent.h"

bool ConsumableItemComponent::Init(const XMLElement* componentNode)
{
	if (componentNode->QueryIntAttribute("uses", &m_uses) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	if (componentNode->QueryIntAttribute("maxstack", &m_maxStack) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
}
