#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "IComponent.h"
#include "../Physics/PhysicsHelpers.h"
#include <functional>
typedef std::function<void(StrongEntityPtr)> reactionFunction;

class ICollisionShape;
class CollisionComponent : public IComponent
{
public:
	~CollisionComponent()
	{
		delete m_collisionShape;
	}

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
	//std::vector<glm::vec2> GetBounds();
	/*
	Sets new bounds of the shape.
	In the case of a polygon, it is the points of the polygon in counter-clockwise order
	In the case of a circle, m_bounds[0] is the center and m_bounds[1] is an arbitrary point on the circle
	*/
	void SetBounds(PhysicsDefs::IShapeData* data);

	PhysicsDefs::CollisionShape GetCollisionShapeType()
	{
		return m_shapeType;
	}
	void SetCollisionShapeType(PhysicsDefs::CollisionShape collShape)
	{
		m_shapeType = collShape;
	}

	const ICollisionShape* GetCollisionShape() const
	{
		return m_collisionShape;
	}

	//Returns position of the collision shape. MAY NOT BE ALLIGNED WITH PARENT OBJECT 
	sf::Vector2f GetPosition();

	bool GetIsSolid()
	{
		return m_isSolid;
	}
	void SetSolid(bool solid)
	{
		m_isSolid = solid;
	}

	sf::Rect<float> GetAABB();

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
	void UpdatePosition(const sf::Vector2f& newPosition);
	//Updates bounds. Arguement is change along x and y axes
	void UpdateBounds(const sf::Vector2f& changes);

private:
	bool CreateShape(XMLElement* shapeNode);
	//The bounding shape
	ICollisionShape* m_collisionShape;

	//If no size is given, this flag is used to initalize width and height from the sprite in PostInit
	bool m_fullBody; 
	bool m_isSolid;
	PhysicsDefs::CollisionShape m_shapeType;

	//Vector containing functions to be called when collision takes place. Functions must be of form "void function(StrongEntityPtr)"
	std::vector<reactionFunction> m_reactionFunctions;

};

#endif