#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Entity.h"

bool TransformComponent:: Init(XMLElement* data)
{
	float x, y, vx, vy;
	
	if ((data->QueryFloatAttribute("x", &x) != tinyxml2::XMLError::XML_SUCCESS) || (data->QueryFloatAttribute("y", &y) != tinyxml2::XMLError::XML_SUCCESS))
	{
		//TODO error
		return false;
	}
	SetPosition(sf::Vector2f(x, y));
	if ((data->QueryFloatAttribute("vx", &vx) != tinyxml2::XMLError::XML_SUCCESS) || (data->QueryFloatAttribute("vy", &vy) != tinyxml2::XMLError::XML_SUCCESS))
	{
		//TODO error
		return false;
	}
	SetSpeed(sf::Vector2f(vx, vy));
	if (data->QueryBoolAttribute("movable", &m_isMovable) != tinyxml2::XMLError::XML_SUCCESS)
	{
		//TODO error
		return false;
	}
	return true;
}

void TransformComponent::Update(double dt)
{
	m_position.x += dt*m_speed.x;
	m_position.y += dt*m_speed.y;
	WeakComponentPtr weakCollisionPtr = m_owner->GetComponent(ComponentBase::GetIDFromName(CollisionComponent::COMPONENT_NAME));
	//If the entity doesn't have a collision box
	if (weakCollisionPtr.expired())
		return;
	//Otherwise, update the box
	StrongComponentPtr strongCollisionPtr = ConvertToStrongPtr(weakCollisionPtr);
	std::shared_ptr<CollisionComponent> collisionBox = CastComponentToDerived<StrongComponentPtr, CollisionComponent>(strongCollisionPtr);
	sf::FloatRect oldBounds = collisionBox->GetBounds();
	collisionBox->SetBounds(sf::FloatRect(oldBounds.left + dt*m_speed.x, oldBounds.top+ dt*m_speed.y, oldBounds.width, oldBounds.height));
	return;
}

void TransformComponent::SetPosition(const sf::Vector2f position)
{
	//TODO
	//UPDATE SPRITE POSITION
	m_position = position;
}
const sf::Vector2f TransformComponent::GetPosition() const
{
	return m_position;
}

void TransformComponent::SetSpeed(const sf::Vector2f speed)
{
	m_speed = speed;
}
const sf::Vector2f TransformComponent::GetSpeed() const
{
	return m_speed;
}

void TransformComponent::SetMovable(const bool movable)
{
	m_isMovable = movable;
}
const bool TransformComponent::GetMovable() const
{
	return m_isMovable;
}

const char* TransformComponent::COMPONENT_NAME = "TransformComponent";