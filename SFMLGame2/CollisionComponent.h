#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "ComponentBase.h"

class CollisionComponent : public ComponentBase
{

public:
	virtual bool Init(const XMLElement* componentNode) override;
	virtual void PostInit() override;

	const char* GetName()
	{
		return COMPONENT_NAME;
	}


	void Update(float dt);
	const static char* COMPONENT_NAME;

	sf::FloatRect GetBounds()
	{
		return m_bounds;
	}
	void SetBounds(sf::FloatRect newBounds)
	{
		m_bounds = newBounds;
	}

private:
	sf::FloatRect m_bounds;
	bool m_fullBody; //If no size is given, this flag is used to initalize width and height from the sprite in PostInit
};

#endif