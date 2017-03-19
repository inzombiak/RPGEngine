#ifndef CIRCLE_SHAPE_H
#define CIRCLE_SHAPE_H

#include "ICollisionShape.h"
#include "..\Utilities\GameDefs.h"

class CircleShape :
	public ICollisionShape
{
public:
	CircleShape(float x, float y, float r);

	~CircleShape();

	const glm::vec2& GetCentroid() const override;
	void SetPosition(glm::vec2 pos) override;
	void UpdatePosition(glm::vec2 change) override;
	std::vector<glm::vec2> GetBounds() const override;

	PhysicsDefs::CircleData* GetData();

private:
	void UpdateAABB() override;

	glm::vec2 m_center;
	double m_radius;
};

#endif