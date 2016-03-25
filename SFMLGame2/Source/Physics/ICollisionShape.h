#ifndef I_COLLISION_SHAPE_H
#define I_COLLISION_SHAPE_H

#include "SFML\Graphics.hpp"
#include "..\Utilities\GameDefs.h"
#include "PhysicsHelpers.h"

class ICollisionShape
{
public:
	ICollisionShape() {};
	virtual ~ICollisionShape() {};

	//Get the AABB bounding this shape
	virtual const PhysicsDefs::AABB& GetAABB() const
	{
		return m_AABB;
	}

	virtual const glm::vec2& GetCentroid() const = 0;
	virtual void SetPosition(glm::vec2 pos) = 0;
	virtual void UpdatePosition(glm::vec2 change) = 0;
	virtual std::vector<glm::vec2> GetBounds() const = 0;

	virtual PhysicsDefs::IShapeData* GetData() = 0;

protected:
	virtual void UpdateAABB() = 0;
	PhysicsDefs::AABB m_AABB;
};

#endif