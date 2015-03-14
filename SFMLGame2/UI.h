#ifndef UI_H
#define UI_H

#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"

#include<string>
using std::string;
class UI
{
public:

	UI();

	void SetPlayerCurrentHP(const int);
	void SetPlayerMaxHP(const int);
	void SetPlayerMaxStamina(const int);
	void SetPlayerCurrentStamina(int);
	
	void ToggleInventory();
	void AddItem(string, int);

	void Draw(sf::RenderWindow&);

private: 

	void UpdateMaxValues();
	void UpdateCurrentValues();

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
};

#endif UI_H