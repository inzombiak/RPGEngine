#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "custom_defininitions.h"
#include <memory>
#include <string>

using std::string;
class InventoryComponent;
class EquipmentComponent;
class PlayerObserverComponent;
class ItemRenderComponent;
class ItemOptionsMenu;
class CharacterWindow;
class UIManager
{
public:
	UIManager();
	void PostInit(); //Grabs initial player data through m_playerObserverS
	void Update(float dt);
	void Draw(sf::RenderWindow&);
	static StrongComponentPtr CreatePlayerObserverComponent(); //Creates player observer
	static StrongItemComponentPtr CreateItemRenderComponent(); //Creates ItemRender component. The reason I separated item and entity rending is because only the UI will render ITems
	void HandleInput(sf::Event event, sf::Vector2i mousePos);

private: 
	void UpdatePlayerVitals();
	void SetPlayerHP(const int, const int);
	void SetPlayerStamina(const int, const int);
	void DrawInventory(sf::RenderWindow& rw);
	void DrawEquipment(sf::RenderWindow& rw);
	/*
	Fix this shit, make it less so much
	*/
	const int CONST_HPFillPixelPerPoint = 2;
	const float CONST_HPBarHeight = 15;
	const float CONST_HPBarX = 0;
	const float CONST_HPBarY = 0;
	const sf::Color CONST_HPBarColor = sf::Color(112, 128, 144, 215);
	const sf::Color CONST_HPBarFillColor = sf::Color(205, 51, 51, 255);

	const int CONST_StaminaFillPixelPerPoint = 2;
	const float CONST_StaminaBarHeight = 15;
	const float CONST_StaminaBarX = 0;
	const float CONST_StaminaBarY = CONST_HPBarY + CONST_HPBarHeight + 5;
	const sf::Color CONST_StaminaBarColor = sf::Color(112, 128, 144, 215);
	const sf::Color CONST_StaminaBarFillColor = sf::Color(0, 139, 0, 255);

	int m_playerMaxHP;
	int m_playerCurrentHP;
	int m_playerMaxStamina;
	int m_playerCurrentStamina;

	sf::Rect<float> m_playerHPBarParameters;
	sf::Rect<float> m_playerHPBarFillParameters;
	sf::Rect<float> m_playerStaminaBarParameters;
	sf::Rect<float> m_playerStaminaBarFillParameters;

	sf::RectangleShape m_playerHPBar;
	sf::RectangleShape m_playerHPBarFill;
	sf::RectangleShape m_playerStaminaBar;
	sf::RectangleShape m_playerStaminaBarFill;

	static std::shared_ptr<PlayerObserverComponent> m_playerObserver; //One player, one observer
	std::shared_ptr<InventoryComponent> m_playerInventory;
	std::shared_ptr<EquipmentComponent> m_playerEquipment;
	static std::vector<std::shared_ptr<ItemRenderComponent>> m_itemRenderComponents;
	sf::Font m_font;

	std::shared_ptr<CharacterWindow> m_characterWindow;
	std::shared_ptr<ItemOptionsMenu> newOpMenu;

	static std::map<Equipment::SlotName, sf::Vector2f> m_slotDisplayCoordinates;

};

#endif