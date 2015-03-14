#include "UI.h"

UI::UI()
{
	m_playerHPBarParameters.left = CONST_HPBarX;
	m_playerHPBarParameters.top = CONST_HPBarY;
	m_playerHPBarParameters.height = CONST_HPBarHeight;

	m_playerHPBar.setPosition(CONST_HPBarX, CONST_HPBarY);

	m_playerHPBarFillParameters.left = CONST_HPBarX;
	m_playerHPBarFillParameters.top = CONST_HPBarY;
	m_playerHPBarFillParameters.height = CONST_HPBarHeight;

	m_playerHPBarFill.setPosition(CONST_HPBarX, CONST_HPBarY);

	m_playerHPBar.setFillColor(CONST_HPBarColor);
	m_playerHPBarFill.setFillColor(CONST_HPBarFillColor);

	m_playerStaminaBarParameters.left = CONST_StaminaBarX;
	m_playerStaminaBarParameters.top = CONST_StaminaBarY;
	m_playerStaminaBarParameters.height = CONST_StaminaBarHeight;

	m_playerStaminaBar.setPosition(CONST_StaminaBarX, CONST_StaminaBarY);

	m_playerStaminaBarFillParameters.left = CONST_StaminaBarX;
	m_playerStaminaBarFillParameters.top = CONST_StaminaBarY;
	m_playerStaminaBarFillParameters.height = CONST_StaminaBarHeight;

	m_playerStaminaBarFill.setPosition(CONST_StaminaBarX, CONST_StaminaBarY);

	m_playerStaminaBar.setFillColor(CONST_StaminaBarColor);
	m_playerStaminaBarFill.setFillColor(CONST_StaminaBarFillColor);

//	m_playerInventory.AddItem("Healing Potion", 1);
	//m_playerInventory.AddItem("Test", 1);
}

void UI::SetPlayerCurrentHP(int newHP)
{
	m_playerCurrentHP = newHP;
	UpdateCurrentValues();
}

void UI::SetPlayerMaxHP(int newMaxHP)
{
	m_playerMaxHP = newMaxHP;
	UpdateMaxValues();
}

void UI::SetPlayerCurrentStamina(int newStamina)
{
	m_playerCurrentStamina = newStamina;
	UpdateCurrentValues();
}

void UI::SetPlayerMaxStamina(int newMaxStamina)
{
	m_playerMaxStamina = newMaxStamina;
	UpdateMaxValues();
}


void UI::UpdateMaxValues()
{
	m_playerHPBarParameters.width = m_playerMaxHP * CONST_HPFillPixelPerPoint;
	m_playerStaminaBarParameters.width = m_playerMaxStamina * CONST_StaminaFillPixelPerPoint;

	m_playerHPBar.setSize(sf::Vector2f(m_playerHPBarParameters.width, m_playerHPBarParameters.height));
	m_playerStaminaBar.setSize(sf::Vector2f(m_playerStaminaBarParameters.width, m_playerStaminaBarParameters.height));
}

void UI::UpdateCurrentValues()
{
	m_playerHPBarFillParameters.width = m_playerCurrentHP * CONST_HPFillPixelPerPoint;
	m_playerStaminaBarFillParameters.width = m_playerCurrentStamina * CONST_StaminaFillPixelPerPoint;

	m_playerHPBarFill.setSize(sf::Vector2f(m_playerHPBarFillParameters.width, m_playerHPBarFillParameters.height));
	m_playerStaminaBarFill.setSize(sf::Vector2f(m_playerStaminaBarFillParameters.width, m_playerStaminaBarFillParameters.height));
}

void UI::AddItem(string name, int quantity)
{
//	m_playerInventory.AddItem(name, quantity);
}

void UI::ToggleInventory()
{
//	m_playerInventory.Display();
}

void UI::Draw(sf::RenderWindow& rw)
{
	
	rw.draw(m_playerHPBar);
	rw.draw(m_playerHPBarFill);
	rw.draw(m_playerStaminaBar);
	rw.draw(m_playerStaminaBarFill);
	//m_playerInventory.Draw(rw);
	
}