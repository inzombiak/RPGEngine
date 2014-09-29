#include "PlayerObserver.h"


//PlayerObserver::PlayerObserver(Player* player, UI* ui)
//{
//	player->SetObserver(this);
//	m_UI = ui;
//}

void PlayerObserver::SetUI(UI* ui)
{
	m_UI = ui;
}

void PlayerObserver::UpdateMaxPlayerHP(int newValue)
{
	m_UI->SetPlayerMaxHP(newValue);
}
void PlayerObserver::UpdateCurrentPlayerHP(int newValue)
{
	m_UI->SetPlayerCurrentHP(newValue);
}
void PlayerObserver::UpdateMaxPlayerStamina(int newValue)
{
	m_UI->SetPlayerMaxStamina(newValue);
}
void PlayerObserver::UpdateCurrentPlayerStamina(int newValue)
{
	m_UI->SetPlayerCurrentStamina(newValue);
}
void PlayerObserver::AddItem(string name, int quantity)
{
	m_UI->AddItem(name, quantity);
}