#ifndef BOX_SHAPE_H
#define BOX_SHAPE_H

#include "ICollisionShape.h"
#include "..\Utilities\Algorithms.h"

class BoxShape :
	public ICollisionShape
{
public:
	BoxShape(float x, float y, float w, float h, int angle);

	~BoxShape();

	const glm::vec2& GetCentroid() const override;
	void SetPosition(glm::vec2 pos) override;
	void UpdatePosition(glm::vec2 change) override;
	std::vector<glm::vec2> GetBounds() const override;

	PhysicsDefs::RectangleData* GetData();

private:
	void UpdateAABB() override;

	int m_angle;
	glm::vec2 m_center;
	double m_width, m_height;
};

#endif