#include "CircleShape.h"
#include "..\Utilities\Algorithms.h"

CircleShape::CircleShape(float x, float y, float r)
{
	m_center.x = x;
	m_center.y = y;
	m_radius = r;

	UpdateAABB();
}

CircleShape::~CircleShape()
{

}

const glm::vec2& CircleShape::GetCentroid() const
{
	return m_center;
}

void CircleShape::SetPosition(glm::vec2  pos)
{
	m_center = pos;
	UpdateAABB();
}

void CircleShape::UpdatePosition(glm::vec2 change)
{
	m_center.x += change.x;
	m_center.y += change.y;

	UpdateAABB();
}


void CircleShape::UpdateAABB()
{
	m_AABB = AABBCalculator::CircleAABB(m_center, m_radius);
}

PhysicsDefs::CircleData* CircleShape::GetData()
{
	PhysicsDefs::CircleData* result = new PhysicsDefs::CircleData();

	result->center = m_center;
	result->radius = m_radius;

	return result;
}

std::vector<glm::vec2> CircleShape::GetBounds() const
{
	std::vector<glm::vec2> result;
	
	result.push_back(m_center);
	result.push_back(glm::vec2(m_center.x + m_radius, m_center.y));

	return result;
}