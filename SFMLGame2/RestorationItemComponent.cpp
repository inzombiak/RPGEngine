#include "RestorationItemComponent.h"


bool RestorationItemComponent::Init(const XMLElement* componentNode)
{
	if (!componentNode->Attribute("effect"))
		return false;
	m_effect = componentNode->Attribute("effect");
	if (componentNode->QueryIntAttribute("amount", &m_amount) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
}

const char* RestorationItemComponent::COMPONENT_NAME = "RestorationItemComponent";