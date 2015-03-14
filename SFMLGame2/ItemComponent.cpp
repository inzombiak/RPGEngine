#include "ItemComponent.h"
#include "RenderComponent.h"

void Item::AddRenderComponent(StrongComponentPtr renderComp)
{
	m_renderComponent = CastComponentToDerived<StrongComponentPtr, RenderComponent>(renderComp);
	m_renderComponent->SetPosition(sf::Vector2f(200, 200));
}
StrongComponentPtr Item::GetRenderComponent()
{
	return m_renderComponent;
}
bool Item::AddItemComponent(ItemComponentID itemID, StrongItemComponentPtr compPtr)
{
	auto it = m_itemComponents.find(itemID);

	if (it != m_itemComponents.end())
		return false;
	m_itemComponents.insert(std::pair<ItemComponentID, StrongItemComponentPtr>(itemID, compPtr));
	return true;
}
WeakItemComponentPtr Item::GetItemComponent(ItemComponentID itemID)
{
	auto it = m_itemComponents.find(itemID);

	if (it == m_itemComponents.end())
		return WeakItemComponentPtr();
	return it->second;
}

bool BaseItemComponent::Init(const XMLElement* componentNode)
{
	if (!componentNode->Attribute("name"))
		return false;
	m_name = componentNode->Attribute("name");
	if (!componentNode->Attribute("description"))
		return false;
	m_description = componentNode->Attribute("description");
	/*if (!componentNode->Attribute("iconfilepath"))
		return false;
	m_iconFilepath = componentNode->Attribute("iconfilepath");*/
	return true;
	
}

bool RestorationItemComponent::Init(const XMLElement* componentNode)
{
	if (!componentNode->Attribute("effect"))
		return false;
	m_effect = componentNode->Attribute("effect");
	if (componentNode->QueryIntAttribute("amount", &m_amount) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	return true;
}

bool ConsumableItemComponent::Init(const XMLElement* componentNode)
{
	if (componentNode->QueryIntAttribute("uses", &m_uses) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	if (componentNode->QueryIntAttribute("maxstack", &m_maxStack) != tinyxml2::XMLError::XML_SUCCESS)
		return false;
	return true;
}

const char* ItemComponent::COMPONENT_NAME = "ItemComponent";
const char* BaseItemComponent::COMPONENT_NAME = "BaseItemComponent";
const char* ItemRenderComponent::COMPONENT_NAME = "ItemRenderComponent";
const char* RestorationItemComponent::COMPONENT_NAME = "RestorationItemComponent";
const char* ConsumableItemComponent::COMPONENT_NAME = "ConsumableItemComponent";