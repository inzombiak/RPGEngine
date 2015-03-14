#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "ComponentBase.h"
#include <functional>

class CollisionComponent : public ComponentBase
{
	typedef std::function<void(StrongEntityPtr)> reactionFunction;
	//typedef int reactionFunction;
public:
	virtual bool Init(XMLElement* componentNode) override;
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
	//Does NOT update transform or sprite positions, use UpdatePosition for post-collision position changes
	void SetBounds(sf::FloatRect newBounds) 
	{
		m_bounds = newBounds;
	}

	bool GetIsSolid()
	{
		return m_isSolid;
	}
	void SetSolid(bool solid)
	{
		m_isSolid = solid;
	}

	void AddReactionFunction(reactionFunction func)
	{
		m_reactionFunctions.push_back(func);
	}
	
	void ApplyCollision(StrongEntityPtr otherEntity)
	{
		for (int i = 0; i < m_reactionFunctions.size(); ++i)
			m_reactionFunctions[i](otherEntity);
	}
	void UpdatePosition(sf::Vector2f newPosition);

private:
	sf::FloatRect m_bounds;
	bool m_fullBody; //If no size is given, this flag is used to initalize width and height from the sprite in PostInit
	bool m_isSolid;
	
	//Vector containing functions to be called when collision takes place. Functions must be of form "void function(StrongEntityPtr)"
	std::vector<reactionFunction> m_reactionFunctions;
};

#endif