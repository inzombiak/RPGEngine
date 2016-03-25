#include "CollisionComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "../Physics//ICollisionShape.h"
#include "../Entity.h"

bool CollisionComponent::Init(XMLElement* data)
{
	string shapeType = data->Attribute("shapeType");
	if (shapeType.empty())
		return false;
	
	auto it = PhysicsDefs::collisionShapeStringToEnumMap.find(shapeType);
	if (it == PhysicsDefs::collisionShapeStringToEnumMap.end())
		return false;
	
	m_shapeType = it->second;

	XMLElement* shapeData = data->FirstChildElement("Shape");
	if (!shapeData)
		return false;

	return CreateShape(shapeData);
}

void CollisionComponent::PostInit()
{
	//Fix this for polygon
	if (m_fullBody && m_shapeType == PhysicsDefs::Rectangle)
	{
		StrongComponentPtr compPtr = ConvertToStrongPtr<IComponent>(m_owner->GetComponent(GetIDFromName(RenderComponent::COMPONENT_NAME)));
		std::shared_ptr<RenderComponent> renderComp = CastComponentToDerived<StrongComponentPtr, RenderComponent>(compPtr);
		auto width = renderComp->GetSprite().getGlobalBounds().width;
		auto height = renderComp->GetSprite().getGlobalBounds().height;

		PhysicsDefs::RectangleData* data = dynamic_cast<PhysicsDefs::RectangleData*>(m_collisionShape->GetData());
		if (!data)
			return;
		
		data->width = width;
		data->height = height;

		delete m_collisionShape;

		m_collisionShape = ShapeFactory::CreateShape(data);
		
		delete data;

	}
}

bool CollisionComponent::CreateShape(XMLElement* data)
{
	float x, y, width = 0, height = 0;

	if (m_shapeType == PhysicsDefs::Rectangle)
	{
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

		PhysicsDefs::RectangleData rectData;

		rectData.center.x = x; 
		rectData.center.y = y;
		rectData.width = width;
		rectData.height = height;
		rectData.angle = 0;
		
		m_collisionShape = ShapeFactory::CreateShape(&rectData);

		return true;
	}

	return false;
}

void CollisionComponent::Update(float dt)
{
	if (!m_owner)
		return;
	WeakComponentPtr weakCompPtr = m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME));
	if (weakCompPtr.expired())
		return;
	StrongComponentPtr compPtr = ConvertToStrongPtr<IComponent>(weakCompPtr);
}

void CollisionComponent::Draw(sf::RenderWindow& rw)
{
	if (m_shapeType == PhysicsDefs::Polygon)
	{
		PhysicsDefs::PolygonData* data = dynamic_cast<PhysicsDefs::PolygonData*>(m_collisionShape->GetData());
		if (!data)
			return;

		//Stores line information
		sf::VertexArray line(sf::Lines, 2);
		line[0].color = sf::Color::Black;
		line[1].color = sf::Color::Black;

		for (int i = 0; i < data->points.size(); ++i)
		{
			//Set position of the endpoints of the axes
			line[0].position = sf::Vector2f(data->points[i].x, data->points[i].y);
			line[1].position = sf::Vector2f(data->points[(i + 1) % data->points.size()].x, data->points[(i + 1) % data->points.size()].y);

			//Draw the line
			rw.draw(line);
		}

		delete data;
	}
	else if (m_shapeType == PhysicsDefs::Circle)
	{
		PhysicsDefs::CircleData* data = dynamic_cast<PhysicsDefs::CircleData*>(m_collisionShape->GetData());
		if (!data)
			return;
		sf::CircleShape circle(data->radius);
		circle.setPosition(sf::Vector2f(data->center.x, data->center.y));

		rw.draw(circle);

		delete data;
	}
	else if (m_shapeType == PhysicsDefs::Rectangle)
	{
		PhysicsDefs::RectangleData* data = dynamic_cast<PhysicsDefs::RectangleData*>(m_collisionShape->GetData());
		if (!data)
			return;

		sf::RectangleShape rect(sf::Vector2f(data->width, data->height));

		rect.setPosition(sf::Vector2f(data->center.x - data->width/2, data->center.y - data->height/2));
		rect.setRotation(data->angle);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(2.0f);
		rw.draw(rect);

		delete data;
	}
}

void CollisionComponent::UpdatePosition(const sf::Vector2f& change)
{
	if (!m_owner)
		return;
	std::shared_ptr<TransformComponent> transComp;
	if (!CheckConvertAndCastPtr(m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME)), transComp))
		return;
	if (!transComp->GetMovable())
		return;
	auto oldTransform = transComp->GetPosition();
	transComp->UpdatePosition(change);
	glm::vec2 glmChange(change.x, change.y);
	m_collisionShape->UpdatePosition(glmChange);
}

void CollisionComponent::UpdateBounds(const sf::Vector2f& change)
{
	glm::vec2 glmChange(change.x, change.y);
	m_collisionShape->UpdatePosition(glmChange);
}

sf::Vector2f CollisionComponent::GetPosition()
{
	sf::Vector2f result;

	auto glmVec = m_collisionShape->GetCentroid();

	result.x = glmVec.x;
	result.y = glmVec.y;

	return result;
}

void CollisionComponent::SetBounds(PhysicsDefs::IShapeData* data)
{
	m_shapeType = data->GetShape();

	switch (m_shapeType)
	{
	case (PhysicsDefs::Rectangle) :
	{
		PhysicsDefs::RectangleData* rectData = dynamic_cast<PhysicsDefs::RectangleData*>(data);
		m_collisionShape = ShapeFactory::CreateShape(rectData);
		return;
	}
	case (PhysicsDefs::Circle) :
	{
		PhysicsDefs::CircleData* circleData = dynamic_cast<PhysicsDefs::CircleData*>(data);
		m_collisionShape = ShapeFactory::CreateShape(circleData);
		return;
	}
	case (PhysicsDefs::Polygon) :
	{
		PhysicsDefs::PolygonData* polyData = dynamic_cast<PhysicsDefs::PolygonData*>(data);
		m_collisionShape = ShapeFactory::CreateShape(polyData);
		return;
	}
	}
}

const char* CollisionComponent::COMPONENT_NAME = "CollisionComponent";