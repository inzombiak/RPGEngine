#ifndef ITEM_COMPONENT_BASE_H
#define ITEM_COMPONENT_BASE_H

#include <map>
#include <memory>
#include <vector>

#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "tinyxml2.h"
#include "custom_defininitions.h"
#include "RenderComponent.h"

using std::map;
using std::string;
using std::vector;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

class RenderComponent;
class Item
{
public:
	void AddRenderComponent(StrongComponentPtr renderComp);
	StrongComponentPtr GetRenderComponent();
	bool AddItemComponent(ItemComponentID itemID, StrongItemComponentPtr compPtr);
	WeakItemComponentPtr GetItemComponent(ItemComponentID itemID);
	void ToggleVisible()
	{
		m_renderComponent->SetVisible(!m_renderComponent->GetVisible());
	}
private:
	map<ItemComponentID, StrongItemComponentPtr> m_itemComponents;
	std::shared_ptr<RenderComponent> m_renderComponent;
};

class ItemComponent
{
public:
	virtual bool Init(const XMLElement* componentNode) = 0;
	virtual void PostInit() = 0;
	void Update(float dt);

	virtual const char* GetName() = 0;

	static ItemComponentID GetIDFromName(const char* name)
	{
		return reinterpret_cast<ItemComponentID>(HashedString::hash_name(name));
	}
	static const char* COMPONENT_NAME;
private:
	
};

class BaseItemComponent : public ItemComponent
{
public:
	virtual bool Init(const XMLElement* componentNode);
	virtual void PostInit(){};
	virtual void Update(float dt) {};
	virtual const char* GetName() override
	{
		return COMPONENT_NAME;
	}

	void SetQuantity(int newQuantity)
	{
		m_quantity = newQuantity;
	}
	int GetQuantity() const
	{
		return m_quantity;
	}

	void SetName(string name)
	{
		m_name = name;
	}
	string GetItemName()
	{
		return m_name;
	}

	void SetDescription(string desc)
	{
		m_description = desc;
	}
	string GetDescription()
	{
		return m_description;
	}

	string GetIconFilepath()
	{
		return m_iconFilepath;
	}

	static const char* COMPONENT_NAME;

private:
	string m_name;
	string m_description;
	string m_iconFilepath;
	int m_quantity;
};

class ItemRenderComponent : public ItemComponent
{
public:
	virtual bool Init(const XMLElement* node) override { return true; };
	virtual void PostInit() override {};
	void Update(float dt);
	virtual const char* GetName() override
	{
		return COMPONENT_NAME;
	}

	//Setter and Getters
	void SetPosition(const sf::Vector2f position)
	{
		m_sprite.setPosition(position);
	}
	const sf::Vector2f GetPosition()
	{
		return m_sprite.getPosition();
	}

	void SetVisible(bool isVisible)
	{
		m_visible = isVisible;
	}
	const bool GetVisible() const
	{
		return m_visible;
	}

	void SetTexture(const sf::Texture& texture, sf::Rect<int> textureRect)
	{
		m_texture = texture;
		m_sprite.setTexture(m_texture);
		m_sprite.setTextureRect(textureRect);
	}
	const sf::Texture& GetTexture() const
	{
		return m_texture;
	}

	void SetSprite(const sf::Sprite sprite)
	{
		m_sprite = sprite;
	}
	const sf::Sprite& GetSprite() const
	{
		return m_sprite;
	}

	void SetTextureRect(const sf::Rect<int> textureBounds)
	{
		m_textureBounds = textureBounds;
	}
	const sf::Rect<int> GetTextureRect() const
	{
		return m_textureBounds;
	}

	const static char* COMPONENT_NAME;

private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Rect<int> m_textureBounds;
	bool m_visible = true;
};

class RestorationItemComponent : public ItemComponent
{
public:
	virtual bool Init(const XMLElement* componentNode) override;
	virtual void PostInit() override {};
	void Update(float dt) {};
	virtual const char* GetName() override
	{
		return COMPONENT_NAME;
	}

	std::string GetEffect()
	{
		return m_effect;
	}

	int GetAmount()
	{
		return m_amount;
	}

	static const char* COMPONENT_NAME;
private:
	std::string m_effect;
	int m_amount;
};

class ConsumableItemComponent : public ItemComponent
{
public:

	virtual bool Init(const XMLElement* componentNode) override;
	virtual void PostInit() override {};
	void Update(float dt) {};
	virtual const char* GetName() override
	{
		return COMPONENT_NAME;
	}

	int GetUsesRemaining()
	{
		return m_uses;
	}
	void SetUses(int uses)
	{
		m_uses = uses;
	}
	int GetMaxStackSize()
	{
		return m_maxStack;
	}
	static const char* COMPONENT_NAME;
private:
	int m_uses;
	int m_maxStack;
};

#endif