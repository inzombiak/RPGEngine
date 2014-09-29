#include "Player.h"

Player::Player()
{
	SetSolid(false);
	SetMovable(true);
	SetVisible(true);
};

void Player::Update(float t)
{
	m_vx = 0;
	sf::Rect<float> collBox{ 0, 2 * GetHeight() / 3, GetWidth(), GetHeight() / 3 };
	SetCollisionRectangle(collBox);
	m_vy = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_vx = 100.0f;
		m_observer->AddItem("Mana Potion", 1);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_vx = -100.0f;
	}
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
	{
		m_vy = -100.0f;
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
	{
		m_vy = 100.0f;

	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
	{
		m_vy = -100.0f;
	}
}

void Player::Draw(sf::RenderWindow& rw)
{
	VisibleGameObject::Draw(rw);
}

void Player::Move(int dx, int dy, float t)
{
	GetSprite().move(m_vx*dx*t, m_vy*dy*t);
}

void Player::End()
{
	//isOnLadder = false;

}
sf::Vector2f Player::GetVelocity()
{
	return sf::Vector2f(m_vx, m_vy);
}

void Player::SetVelocityY(float velocityY)
{
	m_vy = velocityY;
}

float Player::GetVelocityY()
{
	return m_vy;
}

void Player::SetVelocityX(float velocityX)
{
	m_vx = velocityX;
}

float Player::GetVelocityX()
{
	return m_vx;
}

void Player::SetObserver(PlayerObserver* po)
{
	m_observer = po;
}