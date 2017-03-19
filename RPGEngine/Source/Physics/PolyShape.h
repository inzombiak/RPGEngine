#ifndef POLY_SHAPE_H
#define POLY_SHAPE_H

#include "ICollisionShape.h"
#include "..\Utilities\GameDefs.h"

class PolyShape :
	public ICollisionShape
{
public:
	PolyShape(std::vector<glm::vec2> points);

	~PolyShape();

	const glm::vec2& GetCentroid() const override;
	void SetPosition(glm::vec2 pos) override;
	void UpdatePosition(glm::vec2 change) override;
	std::vector<glm::vec2> GetBounds() const override;

	PhysicsDefs::PolygonData* GetData();

private:
	void UpdateAABB() override;

	std::vector<glm::vec2> m_points;
	glm::vec2 m_center;
};

#endif