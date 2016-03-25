#ifndef ITEM_COMPONENT_BASE_H
#define ITEM_COMPONENT_BASE_H

#include <map>
#include <memory>
#include <vector>

#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "..\Utilities\tinyxml2.h"
#include "..\Utilities\GameDefs.h"

using std::map;
using std::string;
using std::vector;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

class Item
{
public:
	bool AddItemComponent(ItemComponentID itemID, StrongItemComponentPtr compPtr);
	WeakItemComponentPtr GetItemComponent(ItemComponentID itemID);
	void Apply(StrongEntityPtr target);
private:
	map<ItemComponentID, StrongItemComponentPtr> m_itemComponents;
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
	std::shared_ptr<Item> m_item;
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

	void SetItemName(string name)
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
	virtual bool Init(const XMLElement* node) override;
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

class EquipableItemComponent : public ItemComponent
{
public:
	virtual bool Init(const XMLElement* componentNode) override;
	virtual void PostInit() override {};
	void Update(float dt) {};
	static const char* COMPONENT_NAME;
	virtual const char* GetName() override
	{
		return COMPONENT_NAME;
	}
	Equipment::SlotName GetSlot()
	{
		return m_slot;
	}

	//Return false is stat doesn't exist, otherwise true
	bool GetModifierByStat(Stats::StatName stat, double& value)
	{
		auto it = m_modifiers.find(stat);
		if (it == m_modifiers.end())
			return false;
		value = it->second;
		return true;
	}

	//Returns copy of map with all modifiers. USE "GetModifierByStat" IF YOU NEED A SPECIFIC STAT MODIFIER
	std::map<Stats::StatName, double> GetAllModifiers()
	{
		return m_modifiers;
	}

	//Removes stat modifier. If stat doesn't exist, does nothing
	void RemoveModifier(Stats::StatName effect)
	{
		auto it = m_modifiers.find(effect);
		if (it != m_modifiers.end())
			m_modifiers.erase(it);
	}

	//Adds a modifier to the item
	void AddModifier(Stats::StatName effect, double amount)
	{
		auto it = m_modifiers.find(effect);
		if (it != m_modifiers.end())
			it->second += amount; //Increase existing modifier
		else
			m_modifiers[effect] = amount; //Add new modifier
	}
private:
	Equipment::SlotName m_slot;
	std::map<Stats::StatName, double> m_modifiers;
};

#endif