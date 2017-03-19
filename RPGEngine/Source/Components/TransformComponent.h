#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "IComponent.h"

class TransformComponent : public IComponent
{
public:
	bool Init(XMLElement* node) override;
	void PostInit() override {};

	const char* GetName()
	{
		return COMPONENT_NAME;
	}


	ComponentID GetID() const
	{
		return GetIDFromName(COMPONENT_NAME);
	}

	void Update(double dt);

	void SetPosition(const sf::Vector2f& position);
	const sf::Vector2f& GetPosition() const;
	void UpdatePosition(const sf::Vector2f& change);

	void SetSpeed(const sf::Vector2f speed);
	const sf::Vector2f GetSpeed() const;

	void SetMovable(const bool movable);
	const bool GetMovable() const;

	const static char* COMPONENT_NAME;

private: 
	sf::Vector2f m_position;
	sf::Vector2f m_speed;
	bool m_isMovable = true;
};

#endif