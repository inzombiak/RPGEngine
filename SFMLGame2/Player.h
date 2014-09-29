#ifndef PLAYER_H
#define PLAYER_H

#include "VisibleGameObject.h"
#include "Inventory.h"
#include "PlayerObserver.h"
#include <iostream>

using namespace std;

class Player : public VisibleGameObject
{
public:
	Player();

	void Update(float);

	void SetVelocityY(float);
	float GetVelocityY();

	void SetVelocityX(float);
	float GetVelocityX();

	void Draw(sf::RenderWindow& rw);

	void Move(int, int, float);
	void End();

	void SetObserver(PlayerObserver*);

	sf::Vector2f GetVelocity();

private:
	PlayerObserver* m_observer;
	int m_maxHp = 100, m_HP = 100;
	int m_maxStamina = 100, m_stamina = 100;
	sf::CircleShape m_shape;
	float m_vx, m_vy;
};

#endif