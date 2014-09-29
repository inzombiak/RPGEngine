#include "VisibleGameObject.h"

void VisibleGameObject::SetPosition(float x, float y)
{
	m_sprite.setPosition(x, y);
}

sf::Vector2f VisibleGameObject::GetPosition() const
{
	return m_sprite.getPosition();
}

void VisibleGameObject::Draw(sf::RenderWindow& rw)
{
	sf::RectangleShape rectShape;
	rectShape.setOutlineThickness(1);
	rectShape.setOutlineColor(sf::Color(250, 150, 100));
	rectShape.setPosition(GetPosition().x + GetCollisionRectangle().left, GetPosition().y + GetCollisionRectangle().top);
	rectShape.setSize(sf::Vector2f(GetCollisionRectangle().width, GetCollisionRectangle().height));
	rectShape.setFillColor(sf::Color::Red);
	rw.draw(m_sprite);
	rw.draw(rectShape);
}

bool VisibleGameObject::Load(std::string& filename)
{
	if (m_image.loadFromFile(filename))
	{
		m_sprite.setTexture(m_image);
		return true;
	}
	else
		return false;
}

bool VisibleGameObject::Load(int w, int h)
{
	sf::Texture t;
	bool r = t.create(w, h);
	m_image = t;
	m_sprite.setTexture(m_image);
	m_sprite.setColor(sf::Color(0, 0, 0, 0));
	//m_sprite.setColor(sf::Color::Red);
	return r;
}

bool VisibleGameObject::Load(sf::Texture& t, sf::Rect<int> r)
{
	m_sprite.setTextureRect(r);
	m_image = t;
	m_sprite.setTexture(t);

	return true;
}

sf::IntRect VisibleGameObject::GetTextureSubRect()
{
	return m_sprite.getTextureRect();
}

sf::Vector2f VisibleGameObject::GetCenterPosition() const 
{
	sf::Vector2f result;
	result.x = GetPosition().x + GetSprite().getLocalBounds().width / 2;
	result.y = GetPosition().y + GetSprite().getLocalBounds().height / 2;

	return result;
}

const sf::Sprite& VisibleGameObject::GetSprite() const
{
	return m_sprite;
}

sf::Sprite& VisibleGameObject::GetSprite()
{
	return m_sprite;
}

sf::Texture& VisibleGameObject::GetTexture()
{
	return m_image;
}

sf::Vector2f VisibleGameObject::GetOrigin() const
{
	return m_sprite.getOrigin();
}

float VisibleGameObject::GetHeight() const
{
	return m_sprite.getGlobalBounds().height;
}

float VisibleGameObject::GetWidth() const
{
	return m_sprite.getGlobalBounds().width;
}

sf::Rect<float> VisibleGameObject::GetBounds() const
{
	return m_sprite.getGlobalBounds();
}
void VisibleGameObject::SetSolid(bool b)
{
	m_isSolid = b;
}

bool VisibleGameObject::GetSolid() const
{
	return m_isSolid;
}

void VisibleGameObject::SetMovable(bool b)
{
	m_isMovable = b;
}

bool VisibleGameObject::GetMovable() const
{
	return m_isMovable;
}

void VisibleGameObject::SetVisible(bool b)
{
	m_isVisible = b;
}

bool VisibleGameObject::GetVisible() const
{
	return m_isVisible;
}

void VisibleGameObject::SetType(string type)
{
	m_Type = type;
}

string VisibleGameObject::GetType() const
{
	return m_Type;
}

void VisibleGameObject::SetCollisionRectangle(sf::Rect<float> rect = sf::Rect<float>(0, 0, 0, 0))
{
	m_collisionBox = rect;
}

sf::Rect<float> VisibleGameObject::GetCollisionRectangle() const
{
	return m_collisionBox;
}

sf::Rect<float> VisibleGameObject::GetGlobalCollisionRectangle() const
{
	sf::Rect<float> result;
	result.left = GetPosition().x + m_collisionBox.left;
	result.top = GetPosition().y + m_collisionBox.top;
	result.width = m_collisionBox.width;
	result.height = m_collisionBox.height;
	return result;
}