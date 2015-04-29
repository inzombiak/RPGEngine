#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "ComponentBase.h"
#include <functional>
typedef std::function<void(StrongEntityPtr)> reactionFunction;
//class ICollisionComponent : public ComponentBase
//{
//public:
//	typedef std::function<void(StrongEntityPtr)> reactionFunction;
//	virtual sf::FloatRect GetBounds() = 0;
//	static const char* COMPONENT_NAME;
//	//Does NOT update transform or sprite positions, use UpdatePosition for post-collision position changes
//	virtual void SetBounds(sf::FloatRect newBounds) = 0;
//	virtual bool GetIsSolid() = 0;
//	virtual void SetSolid(bool solid) = 0;
//	virtual void AddReactionFunction(reactionFunction func) = 0;
//	virtual void ApplyCollision(StrongEntityPtr otherEntity) = 0;
//	virtual void UpdatePosition(sf::Vector2f newPosition) = 0;
//};

class CollisionComponent : public ComponentBase
{
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


//class PolylineCollisionComponent : public ICollisionComponent
//{
//public:
//	virtual bool Init(XMLElement* componentNode) override;
//	virtual void PostInit() override;
//
//	static const char* GetName()
//	{
//		return COMPONENT_NAME;
//	}
//
//	void Update(float dt);
//	const static char* COMPONENT_NAME;
//
//	sf::FloatRect GetBounds()
//	{
//		return m_bounds;
//	}
//	//Does NOT update transform or sprite positions, use UpdatePosition for post-collision position changes
//	void SetBounds(sf::FloatRect newBounds)
//	{
//		m_bounds = newBounds;
//	}
//
//	bool GetIsSolid()
//	{
//		return m_isSolid;
//	}
//	void SetSolid(bool solid)
//	{
//		m_isSolid = solid;
//	}
//
//	void AddReactionFunction(reactionFunction func)
//	{
//		m_reactionFunctions.push_back(func);
//	}
//
//	void ApplyCollision(StrongEntityPtr otherEntity)
//	{
//		for (int i = 0; i < m_reactionFunctions.size(); ++i)
//			m_reactionFunctions[i](otherEntity);
//	}
//	void UpdatePosition(sf::Vector2f newPosition);
//
//private:
//	sf::FloatRect m_bounds;
//	bool m_fullBody; //If no size is given, this flag is used to initalize width and height from the sprite in PostInit
//	bool m_isSolid;
//	std::vector<std::pair<int, int>> m_points;
//	//Vector containing functions to be called when collision takes place. Functions must be of form "void function(StrongEntityPtr)"
//	std::vector<reactionFunction> m_reactionFunctions;
//};

#endif