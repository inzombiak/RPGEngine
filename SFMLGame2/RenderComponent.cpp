#include "Entity.h"
#include "RenderComponent.h"
#include "TransformComponent.h"

bool RenderComponent::Init(const XMLElement* node)
{
	
	sf::Texture tex;
	if (!tex.loadFromFile(node->Attribute("filepath")))
	{
		//TODO add error message
		return false;
	}
	SetTexture(tex, sf::Rect<int>(0, 0,tex.getSize().x, tex.getSize().y));
	if(node->QueryBoolAttribute("visible", &m_visible) != tinyxml2::XMLError::XML_SUCCESS)
	{
		//TODO add error message
		return false;
	}
	if (node->QueryIntAttribute("depth", &m_depth) != tinyxml2::XMLError::XML_SUCCESS)
	{
		//TODO add error message
		return false;
	}
	return true;
}

void RenderComponent::PostInit()
{
	StrongComponentPtr compPtr = ConvertToStrongPtr<ComponentBase>(m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME)));
	std::shared_ptr<TransformComponent> transComp = CastComponentToDerived<TransformComponent>(compPtr);
	float x = transComp->GetPosition().x, y = transComp->GetPosition().y;

	m_sprite.setPosition(x, y);
}

void RenderComponent::Update(float dt)
{
	StrongComponentPtr compPtr = ConvertToStrongPtr<ComponentBase>(m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME)));
	std::shared_ptr<TransformComponent> transComp = CastComponentToDerived<TransformComponent>(compPtr);
	float x = transComp->GetPosition().x, y = transComp->GetPosition().y;

	m_sprite.setPosition(x, y);
}

void RenderComponent::SetPosition(const sf::Vector2f position)
{
	m_sprite.setPosition(position);
}

void RenderComponent::SetTexture(const sf::Texture& texture, sf::Rect<int> textureRect)
{
	m_texture = texture;
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(textureRect);
}
const sf::Texture& RenderComponent::GetTexture() const
{
	return m_texture;
}

void RenderComponent::SetVisible(bool isVisible)
{
	m_visible = isVisible;
}
const bool RenderComponent::GetVisible() const
{
	return m_visible;
}

void RenderComponent::SetSprite(const sf::Sprite sprite)
{
	m_sprite = sprite;
}
const sf::Sprite& RenderComponent::GetSprite() const
{
	return m_sprite;
}

void RenderComponent::SetTextureRect(const sf::Rect<int> textureBounds)
{
	m_textureBounds = textureBounds;
}
const sf::Rect<int> RenderComponent::GetTextureRect() const
{
	return m_textureBounds;
}

void RenderComponent::SetDepth(int depth)
{
	m_depth = depth;
}
const int RenderComponent::GetDepth() const
{
	return m_depth;
}

const char* RenderComponent::COMPONENT_NAME = "RenderComponent";