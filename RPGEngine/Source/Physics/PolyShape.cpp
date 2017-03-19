#include "PolyShape.h"

#include "..\Utilities\Algorithms.h"

PolyShape::PolyShape(std::vector<glm::vec2> points)
{
	m_points = points;

	m_center = PolylineHelper::CalcPolylineCentroid(points);

	UpdateAABB();
}

PolyShape::~PolyShape()
{

}

const glm::vec2& PolyShape::GetCentroid() const
{
	return m_center;
}

void PolyShape::SetPosition(glm::vec2 pos)
{
	float xInc = pos.x - m_center.x;
	float yInc = pos.y - m_center.y;

	for (int i = 0; i < m_points.size(); ++i)
	{
		m_points[i].x += xInc;
		m_points[i].y += yInc;
	}

	m_center = pos;
	UpdateAABB();
}

void PolyShape::UpdatePosition(glm::vec2 change)
{
	for (int i = 0; i < m_points.size(); ++i)
	{
		m_points[i].x += change.x;
		m_points[i].y += change.y;
	}

	m_center.x += change.x;
	m_center.y += change.y;

	UpdateAABB();
}

void PolyShape::UpdateAABB()
{
	m_AABB = AABBCalculator::PolylineAABB(m_points);
}

PhysicsDefs::PolygonData* PolyShape::GetData()
{
	PhysicsDefs::PolygonData* result = new PhysicsDefs::PolygonData();

	result->points = m_points;

	return result;
}

std::vector<glm::vec2> PolyShape::GetBounds() const
{
	return m_points;
}