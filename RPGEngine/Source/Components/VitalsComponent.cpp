#include "VitalsComponent.h"

bool VitalsComponent::Init(XMLElement* node)
{
	XMLElement* pVitals = node->FirstChildElement("Vitals");
	XMLElement* pVital = pVitals->FirstChildElement("Vital");
	VitalsComponent::Vital newVital;
	while (pVital)
	{
		if (pVital->QueryIntAttribute("max", &newVital.max) != tinyxml2::XMLError::XML_SUCCESS)
			return false;
		if (pVital->QueryIntAttribute("current", &newVital.current) != tinyxml2::XMLError::XML_SUCCESS)
			return false;
		if (!pVital->Attribute("type"))
			return false;
		std::string type = pVital->Attribute("type");
		if (type.compare("Health") == 0)
			newVital.type = VitalsComponent::VitalType::Health;
		else if (type.compare("Mana") == 0)
			newVital.type = VitalsComponent::VitalType::Mana;
		else if (type.compare("Stamina") == 0)
			newVital.type = VitalsComponent::VitalType::Stamina;
		else
			return false;
		m_vitals[newVital.type] = newVital;
		pVital = pVital->NextSiblingElement("Vital");
	}
	return true;
}

const char* VitalsComponent::COMPONENT_NAME = "VitalsComponent";