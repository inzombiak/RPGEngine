#include "StatComponent.h"

bool StatComponent::Init(XMLElement* node)
{
	XMLElement* pStats = node->FirstChildElement("Stats");
	XMLElement* pStat = pStats->FirstChildElement("Stat");
	int value;
	while (pStat)
	{
		if (!pStat->Attribute("name"))
			return false;
		if (pStat->QueryIntAttribute("value", &value) != tinyxml2::XMLError::XML_SUCCESS)
			return false;
		m_stats[Stats::statMap[pStat->Attribute("name")]] = value;
		pStat = pStat->NextSiblingElement("Stat");
	}
	return true;
}

const char* StatComponent::COMPONENT_NAME = "StatComponent";