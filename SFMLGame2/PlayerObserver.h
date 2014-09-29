#ifndef PLAYER_OBSERVER_H
#define PLAYER_OBSERVER_H

#include "UI.h"


class PlayerObserver
{
public:
	//PlayerObserver(Player*, UI*);

	void SetUI(UI*);
	void UpdateMaxPlayerHP(int);
	void UpdateCurrentPlayerHP(int);

	void UpdateMaxPlayerStamina(int);
	void UpdateCurrentPlayerStamina(int);

	void AddItem(string, int);

private:
	UI* m_UI;
};

#endif