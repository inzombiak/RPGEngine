#ifndef VISIBLEGAMEOBJECT_H
#define VISIBLEGAMEOBJECT_H

#include "SFML\Graphics.hpp"
#include <string>

using std::string;

class VisibleGameObject
{
public:
	//virtual ~VisibleGameObject();

	virtual bool Load(std::string&);
	virtual bool Load(sf::Texture&, sf::Rect<int>);
	virtual bool Load(int width, int height);
	virtual void Update(float) {} ;
	virtual void Move(int, int, float) {};
	virtual void SetPosition(float, float);
	virtual void Draw(sf::RenderWindow&);
	virtual void ResolveCollision(){}

	virtual sf::IntRect GetTextureSubRect();

	virtual const sf::Sprite& GetSprite() const;
	virtual sf::Sprite& GetSprite();

	virtual sf::Vector2f GetPosition() const;
	virtual sf::Vector2f GetCenterPosition() const;
	virtual sf::Rect<float> GetBounds() const;
	virtual sf::Vector2f GetOrigin() const;
	virtual float GetHeight() const;
	virtual float GetWidth() const;

	virtual void SetGrounded(bool){};
	virtual bool GetGrounded() const{ return true; };

	virtual void SetSolid(bool);
	virtual bool GetSolid() const;
	
	virtual void SetMovable(bool);
	virtual bool GetMovable() const;
	
	virtual sf::Texture& GetTexture();

	virtual void SetVisible(bool);
	virtual bool GetVisible() const;

	virtual void SetType(string);
	virtual string GetType() const;

	virtual void SetCollisionRectangle(sf::Rect<float>);
	virtual sf::Rect<float> GetCollisionRectangle() const;

	virtual sf::Rect<float> GetGlobalCollisionRectangle() const;

private:
	sf::Sprite m_sprite;
	sf::Texture m_image;
	std::string m_filename;
	sf::Rect<float> m_collisionBox;
	bool m_isSolid;
	bool m_isMovable;
	bool m_isVisible;
	string m_Type;
};

#endif