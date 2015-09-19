#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "ComponentBase.h"
#include <functional>
typedef std::function<void(StrongEntityPtr)> reactionFunction;

namespace CollisionShape
{
	enum CollisionShape
	{
		Polygon,
		Rectangle,
		Circle,
	};
	
	static std::map<string, CollisionShape> collisionShapeStringToEnumMap =
	{
		{ "Polygon", CollisionShape::Polygon },
		{ "Rectangle", CollisionShape::Rectangle },
		{ "Circle", CollisionShape::Circle },
	};
}

class CollisionComponent : public ComponentBase
{
public:
	virtual bool Init(XMLElement* componentNode) override;
	virtual void PostInit() override;

	const char* GetName()
	{
		return COMPONENT_NAME;
	}

	//Used for debug drawing
	void Draw(sf::RenderWindow& rw);
	void Update(float dt);
	const static char* COMPONENT_NAME;

	/*
	Returns bounds of the shape.
	In the case of a polygon, it is the points of the polygon in counter-clockwise order
	In the case of a circle, bounds[0] is the center and bounds[1] is an arbitrary point on the circle
	*/
	std::vector<sf::Vector2f> GetBounds()
	{
		return m_bounds;
	}
	/*
	Sets new bounds of the shape.
	In the case of a polygon, it is the points of the polygon in counter-clockwise order
	In the case of a circle, m_bounds[0] is the center and m_bounds[1] is an arbitrary point on the circle
	*/
	void SetBounds(std::vector<sf::Vector2f> newBounds)
	{
		m_bounds = newBounds;
		if (m_collisionShape == CollisionShape::Circle)
			m_position = newBounds[0];
		else
			m_position = GetCentroid();
	}

	CollisionShape::CollisionShape GetCollisionShape()
	{
		return m_collisionShape;
	}
	void SetCollisionShape(CollisionShape::CollisionShape collShape)
	{
		m_collisionShape = collShape;
	}

	//Returns position of the collision shape. MAY NOT BE ALLIGNED WITH PARENT OBJECT 
	sf::Vector2f GetPosition()
	{
		return m_position;
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
	//Updates position of the object
	void UpdatePosition(sf::Vector2f newPosition);
	//Updates bounds. Arguement is change along x and y axes
	void UpdateBounds(sf::Vector2f changes)
	{
		m_position += changes;
		for (int i = 0; i < m_bounds.size(); ++i)
			m_bounds[i] += changes;
	}
private:
	//Calculates centroid of the shape, not called if shape is a circle
	sf::Vector2f GetCentroid();

	/*
	Bounds of the shape.
	In the case of a polygon, it is the points of the polygon in counter-clockwise order
	In the case of a circle, m_bounds[0] is the center and m_bounds[1] is an arbitrary point on the circle
	*/
	std::vector<sf::Vector2f> m_bounds;
	//Position of the collison bounds. Center of the shape
	sf::Vector2f m_position;

	//If no size is given, this flag is used to initalize width and height from the sprite in PostInit
	bool m_fullBody; 
	bool m_isSolid;
	CollisionShape::CollisionShape m_collisionShape;
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