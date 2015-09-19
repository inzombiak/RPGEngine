#ifndef CHARACTER_WINDOW_H
#define CHARACTER_WINDOW_H

#include "SFML\Graphics.hpp"
#include "custom_defininitions.h"
#include <sstream>
#include <iomanip>

class StatComponent;
class VitalsComponenet;
class ItemRenderComponent;
class EquipmentComponent;
class CharacterWindow
{
public:
	CharacterWindow(sf::Font font, int fontSize)
	{
		m_font = font;
		m_fontSize = fontSize;
	}

	//Initialize the variables of the window from the XML
	bool Init();

	//Pass a pointer to the players stats, if this isn't called window will not function properly
	void SetStats(std::shared_ptr<StatComponent> stats)
	{
		m_stats = stats;
	}

	//Returns bounds of the window
	sf::FloatRect GetBounds() const
	{
		return m_window.getGlobalBounds();
	}

	void Update(float dt);

	//Pass a map of render componenets of items held by player and draw the character window
	void Draw(const std::map < Equipment::SlotName, std::shared_ptr<ItemRenderComponent>>& , sf::RenderWindow&);

private:

	//Text font and size
	sf::Font m_font;
	int m_fontSize;


	struct StatField
	{
		sf::Vector2f labelPosition;
		sf::Vector2f valuePosition;
		sf::Text name;
		sf::Text value;
	};


	const string XML_FILE = "data/UI.xml";

	//Background shape
	sf::RectangleShape m_window;

	//Character model texture
	sf::Texture m_characterTexture;
	//Character model sprite
	sf::Sprite m_characterSprite;

	//Positions for equipment
	std::map<Equipment::SlotName, sf::Vector2f> m_equipmentSlotPositions;

	//Positions for stats
	std::map<Stats::StatName, StatField> m_statFields;
	//Pointer to characters stat component
	std::shared_ptr<StatComponent> m_stats;
};

#endif