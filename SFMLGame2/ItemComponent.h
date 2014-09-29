//#include <string>
//
//#include "SFML\Graphics.hpp"
//
//
//enum ItemType
//{
//	Healing,
//	Mana,
//	Stamina,
//	Buff,
//	Special,
//	Melee,
//	Bow,
//	Staff,
//	Shield,
//	Head,
//	Chest,
//	Arms,
//	Legs,
//	Ring,
//	Necklace,
//};
//
//
//class ItemInterface
//{
//public:
//	ItemComponent();
//	virtual void Set();
//
//private:
//	string m_name;
//
//};
//
//class Name : public ItemComponent
//{
//
//protected:
//	std::string m_name;
//};
//
//class Description : public ItemComponent
//{
//protected:
//	std::string m_description;
//};
//
//class Type : public ItemComponent
//{
//protected:
//	ItemType m_type;
//};
//
//class Renderable : public ItemComponent
//{
//protected:
//	sf::Sprite m_sprite;
//	sf::Texture m_texture;
//};
//
//class Weapon : public ItemComponent
//{
//protected:
//	int m_damage;
//};
//
//class Consumable : public ItemComponent
//{
//protected:
//	std::string m_effect;
//	int m_amount;
