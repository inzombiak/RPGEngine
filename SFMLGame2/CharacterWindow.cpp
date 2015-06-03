#include "CharacterWindow.h"
#include "StatComponent.h"
#include "EquipmentComponent.h"

#include "tinyxml2.h"

using tinyxml2::XMLElement;

bool CharacterWindow::Init()
{
	tinyxml2::XMLDocument xmlDoc;
	if (xmlDoc.LoadFile(XML_FILE.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	tinyxml2::XMLNode* pRoot = xmlDoc.FirstChild();

	//Make sure all necessary elements exist
	XMLElement* pCharWindow = pRoot->FirstChildElement("CharacterWindow");
	if (!pCharWindow)
		return false;

	XMLElement* pWindow = pCharWindow->FirstChildElement("Window");
	if (!pWindow)
		return false;

	XMLElement* pCharacterModel = pCharWindow->FirstChildElement("CharaterModel");
	if (!pCharacterModel)
		return false;

	XMLElement* pEquipment = pCharWindow->FirstChildElement("Equipment");
	if (!pEquipment)
		return false;

	XMLElement* pStats = pCharWindow->FirstChildElement("Stats");
	if (!pStats)
		return false;

	double windowX, windowY, windowWidth, windowHeight;
	double x, y, w, h;

	
	//Create window
	if (pWindow->QueryDoubleAttribute("x", &windowX) != tinyxml2::XMLError::XML_SUCCESS)
		return false; 
	if (pWindow->QueryDoubleAttribute("y", &windowY) != tinyxml2::XMLError::XML_SUCCESS)
		return false; 
	if (pWindow->QueryDoubleAttribute("width", &windowWidth) != tinyxml2::XMLError::XML_SUCCESS)
		return false; 
	if (pWindow->QueryDoubleAttribute("height", &windowHeight) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	m_window.setSize(sf::Vector2f(windowWidth, windowHeight));
	m_window.setPosition(sf::Vector2f(windowX, windowY));
	m_window.setFillColor(sf::Color::Black);

	//Load character model
	string temp = pCharacterModel->Attribute("filepath");
	if (!m_characterTexture.loadFromFile(pCharacterModel->Attribute("filepath")))
		return false;
	if (pCharacterModel->QueryDoubleAttribute("x", &x) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	if (pCharacterModel->QueryDoubleAttribute("y", &y) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	m_characterSprite.setTexture(m_characterTexture);
	m_characterSprite.setPosition(sf::Vector2f(windowX + x, windowY + y));

	//Load equipment slots
	XMLElement* pSlot = pEquipment->FirstChildElement("Slot");

	while (pSlot)
	{
		if (pSlot->Attribute("name") &&
			pSlot->QueryDoubleAttribute("x", &x) == tinyxml2::XMLError::XML_SUCCESS &&
			pSlot->QueryDoubleAttribute("y", &y) == tinyxml2::XMLError::XML_SUCCESS)
		{
			auto it = Equipment::slotMap.find(pSlot->Attribute("name"));
			if (it != Equipment::slotMap.end())
			{
				m_equipmentSlotPositions[it->second] = sf::Vector2f(windowX + x, windowY + y);
			}
		}
		pSlot = pSlot->NextSiblingElement("Slot");
	}

	//Load stat slots
	XMLElement* pStat = pStats->FirstChildElement("Stat");
	double valueX, valueY;
	StatField newStatField;
	newStatField.name.setFont(m_font);
	newStatField.name.setCharacterSize(m_fontSize);
	newStatField.value.setFont(m_font);
	newStatField.value.setCharacterSize(m_fontSize);
	while (pStat)
	{
		if (pStat->Attribute("name"))
		{
			newStatField.name.setString(pStat->Attribute("name"));
			auto it = Stats::statMap.find(pStat->Attribute("name"));
			if (it != Stats::statMap.end())
			{
				if (pStat->QueryDoubleAttribute("labelX", &x) == tinyxml2::XMLError::XML_SUCCESS &&
					pStat->QueryDoubleAttribute("labelY", &y) == tinyxml2::XMLError::XML_SUCCESS &&
					pStat->QueryDoubleAttribute("valueX", &valueX) == tinyxml2::XMLError::XML_SUCCESS &&
					pStat->QueryDoubleAttribute("valueY", &valueY) == tinyxml2::XMLError::XML_SUCCESS)
				{	
					newStatField.labelPosition = sf::Vector2f(windowX + x, windowY + y);
					newStatField.valuePosition = sf::Vector2f(windowX + valueX, windowY + valueY);
					newStatField.name.setPosition(sf::Vector2f(windowX + x, windowY + y));
					newStatField.value.setPosition(sf::Vector2f(windowX + valueX, windowY + valueY));

					m_statFields[it->second] = newStatField;
				}
			}
		}

		pStat = pStat->NextSiblingElement("Stat");
	}

}

void CharacterWindow::Update(float dt)
{
	//Update stats !EDIT maybe set to update message
	double statValue;
	for (std::map<Stats::StatName, StatField>::iterator it = m_statFields.begin(); it != m_statFields.end(); ++it)
	{
		if (m_stats->HasStat(it->first))
		{
			statValue = m_stats->GetStat(it->first);
			it->second.value.setString(std::to_string(statValue));
		}
	}
}

void CharacterWindow::Draw(const std::map < Equipment::SlotName, std::shared_ptr<ItemRenderComponent>>& equipmentMap, sf::RenderWindow& rw)
{

	//Draw background window
	rw.draw(m_window);

	//Draw player model
	rw.draw(m_characterSprite);

	//Draw equipment
	for (auto it = equipmentMap.begin(); it != equipmentMap.end(); ++it)
	{
		it->second->SetPosition(m_equipmentSlotPositions[it->first]);
		rw.draw(it->second->GetSprite());
	}

	//Draw stats
	for (auto it = m_statFields.begin(); it != m_statFields.end(); ++it)
	{
		rw.draw(it->second.name);
		rw.draw(it->second.value);
	}
}

