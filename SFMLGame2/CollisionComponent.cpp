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

	SetBounds(sf::FloatRect(x, y, width, height));
	return true;
}

void CollisionComponent::PostInit()
{
	if (m_fullBody)
	{
		StrongComponentPtr compPtr = ConvertToStrongPtr<ComponentBase>(m_owner->GetComponent(GetIDFromName(RenderComponent::COMPONENT_NAME)));
		std::shared_ptr<RenderComponent> renderComp = CastComponentToDerived<StrongComponentPtr, RenderComponent>(compPtr);
		sf::FloatRect bounds = GetBounds();
		bounds.width = renderComp->GetSprite().getGlobalBounds().width;
		bounds.height = renderComp->GetSprite().getGlobalBounds().height;
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
	std::shared_ptr<TransformComponent> transComp = CastComponentToDerived<StrongComponentPtr, TransformComponent>(compPtr);
	m_bounds.left = transComp->GetPosition().x;
	m_bounds.top = transComp->GetPosition().y;
}

void CollisionComponent::UpdatePosition(sf::Vector2f newPosition)
{
	m_bounds.left = newPosition.x;
	m_bounds.top = newPosition.y;

	if (!m_owner)
		return;
	WeakComponentPtr weakCompPtr = m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME));
	if (weakCompPtr.expired())
		return;
	StrongComponentPtr compPtr = ConvertToStrongPtr<ComponentBase>(weakCompPtr);
	std::shared_ptr<TransformComponent> transComp = CastComponentToDerived<StrongComponentPtr, TransformComponent>(compPtr);
	transComp->SetPosition(newPosition);
}

const char* CollisionComponent::COMPONENT_NAME = "CollisionComponent";