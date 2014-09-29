//#include "Item.h"
//
//Item::Item(string newName, string newDesc, string icon, ItemType type)
//{
//	m_name = newName;
//	m_description = newDesc;
//	m_type = type;
//	Load(icon);
//}
//
//bool Item::Load(std::string& filename)
//{
//	if (m_image.loadFromFile(filename))
//	{
//		m_sprite.setTexture(m_image);
//		return true;
//	}
//	else
//		return false;
//}
//
//void Item::Draw(sf::RenderWindow& rw)
//{
//	rw.draw(m_sprite);
//}
//
//sf::Texture& Item::GetTexture()
//{
//	return m_image;
//}
//
//string Item::GetName()
//{
//	return m_name;
//}
//
//string Item::GetDescription()
//{
//	return m_description;
//}