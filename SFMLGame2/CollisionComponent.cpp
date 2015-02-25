#include "CollisionComponent.h"
#include "RenderComponent.h"
#include "Entity.h"

bool CollisionComponent::Init(const XMLElement* data)
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

	SetBounds(sf::FloatRect(x, y, width, height));
	return true;
}

void CollisionComponent::PostInit()
{
	if (m_fullBody)
	{
		StrongComponentPtr compPtr = ConvertToStrongPtr<ComponentBase>(m_owner->GetComponent(GetIDFromName(RenderComponent::COMPONENT_NAME)));
		std::shared_ptr<RenderComponent> renderComp = CastComponentToDerived<RenderComponent>(compPtr);
		sf::FloatRect bounds = GetBounds();
		bounds.width = renderComp->GetSprite().getGlobalBounds().width;
		bounds.height = renderComp->GetSprite().getGlobalBounds().height;
		SetBounds(bounds);
	}
}

void CollisionComponent::Update(float dt)
{

}

const char* CollisionComponent::COMPONENT_NAME = "CollisionComponent";