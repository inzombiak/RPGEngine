#include "BoxShape.h"

BoxShape::BoxShape(float x, float y, float w, float h, int angle)
{
	m_center.x = x + w/2;
	m_center.y = y + h/2;
	m_width = w;
	m_height = h;
	m_angle = angle;

	UpdateAABB();
}

BoxShape::~BoxShape()
{

}

const glm::vec2& BoxShape::GetCentroid() const
{
	return m_center;
}

void BoxShape::SetPosition(glm::vec2 pos)
{
	m_center = pos;
	UpdateAABB();
}

void BoxShape::UpdatePosition(glm::vec2 change)
{
	m_center.x += change.x;
	m_center.y += change.y;

	UpdateAABB();
}

void BoxShape::UpdateAABB()
{
	m_AABB = AABBCalculator::RectangleAABB(m_center, m_width, m_height, m_angle);
}

PhysicsDefs::RectangleData* BoxShape::GetData()
{
	PhysicsDefs::RectangleData* result = new PhysicsDefs::RectangleData();

	result->center = m_center;
	result->angle = m_angle;
	result->height = m_height;
	result->width = m_width;

	return result;
}

std::vector<glm::vec2> BoxShape::GetBounds() const
{
	std::vector<glm::vec2> result;

	result.push_back(glm::vec2(m_center.x - m_width / 2, m_center.y - m_height / 2));
	result.push_back(glm::vec2(m_center.x + m_width / 2, m_center.y - m_height / 2));
	result.push_back(glm::vec2(m_center.x + m_width / 2, m_center.y + m_height / 2));
	result.push_back(glm::vec2(m_center.x - m_width / 2, m_center.y + m_height / 2));

	return result;
}