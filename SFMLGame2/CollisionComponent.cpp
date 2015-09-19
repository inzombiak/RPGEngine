#include "CollisionComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Entity.h"

bool CollisionComponent::Init(XMLElement* data)
{
	float x, y, width = 0, height = 0;
	if ((data->QueryFloatAttribute("x", &x) != tinyxml2::XMLError::XML_SUCCESS) || (data->QueryFloatAttribute("y", &y) != tinyxml2::XMLError::XML_SUCCESS))
	{
		//TODO error
		return false;
	}
	if ((data->QueryFloatAttribute("width", &width) != tinyxml2::XMLError::XML_SUCCESS) || (data->QueryFloatAttribute("height", &height) != tinyxml2::XMLError::XML_SUCCESS))
	{
		//TODO error
		m_fullBody = true;
	}
	if (data->QueryBoolAttribute("solid", &m_isSolid) != tinyxml2::XMLError::XML_SUCCESS)
		return false;

	m_collisionShape = CollisionShape::Rectangle;

	//Fix this, what if object is using polygon box?
	std::vector<sf::Vector2f> collisionBounds;
	collisionBounds.push_back(sf::Vector2f(x, y));
	collisionBounds.push_back(sf::Vector2f(x, y + height));
	collisionBounds.push_back(sf::Vector2f(x + width, y + height));
	collisionBounds.push_back(sf::Vector2f(x + width, y));

	SetBounds(collisionBounds);
	return true;
}

void CollisionComponent::PostInit()
{
	//Fix this for polygon
	if (m_fullBody && m_collisionShape == CollisionShape::Rectangle)
	{
		StrongComponentPtr compPtr = ConvertToStrongPtr<ComponentBase>(m_owner->GetComponent(GetIDFromName(RenderComponent::COMPONENT_NAME)));
		std::shared_ptr<RenderComponent> renderComp = CastComponentToDerived<StrongComponentPtr, RenderComponent>(compPtr);
		std::vector<sf::Vector2f> bounds = GetBounds();
		auto width = renderComp->GetSprite().getGlobalBounds().width;
		auto height = renderComp->GetSprite().getGlobalBounds().height;

		bounds[1].y = bounds[0].y + height;
		bounds[2].x = bounds[0].x + width;
		bounds[2].y = bounds[0].y + height;
		bounds[3].x = bounds[0].x + width;

		SetBounds(bounds);
	}
}

void CollisionComponent::Update(float dt)
{
	if (!m_owner)
		return;
	WeakComponentPtr weakCompPtr = m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME));
	if (weakCompPtr.expired())
		return;
	StrongComponentPtr compPtr = ConvertToStrongPtr<ComponentBase>(weakCompPtr);
}

void CollisionComponent::Draw(sf::RenderWindow& rw)
{
	//Stores line information
	sf::VertexArray line(sf::Lines, 2);
	line[0].color = sf::Color::Black;
	line[1].color = sf::Color::Black;

	sf::Vector2f edge;

	for (int i = 0; i < m_bounds.size(); ++i)
	{
		//Set position of the endpoints of the axes
		line[0].position = m_bounds[i];
		line[1].position = m_bounds[(i + 1) % m_bounds.size()];

		//Draw the line
		rw.draw(line);
	}
}

void CollisionComponent::UpdatePosition(sf::Vector2f newPosition)
{
	if (!m_owner)
		return;
	std::shared_ptr<TransformComponent> transComp;
	if (!CheckConvertAndCastPtr(m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME)), transComp))
		return;
	if (!transComp->GetMovable())
		return;
	auto oldTransform = transComp->GetPosition();
	transComp->SetPosition(sf::Vector2f(oldTransform.x + newPosition.x, oldTransform.y + newPosition.y));
}

sf::Vector2f CollisionComponent::GetCentroid()
{
	auto points = m_bounds;
	int nextIndex;
	sf::Vector2f centroid;
	double area = 0;

	for (int i = 0; i < points.size(); ++i)
	{
		nextIndex = (i + 1) % points.size();
		area += (points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);
	}
	area /= 2;

	for (int i = 0; i < points.size(); ++i)
	{
		nextIndex = (i + 1) % points.size();
		centroid.x += (points[i].x + points[nextIndex].x)*(points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);
		centroid.y += (points[i].y + points[nextIndex].y)*(points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);

	}

	centroid.x /= (6 * area);
	centroid.y /= (6 * area);

	return centroid;
}

const char* CollisionComponent::COMPONENT_NAME = "CollisionComponent";