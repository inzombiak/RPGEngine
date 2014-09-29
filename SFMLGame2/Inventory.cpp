#include "Inventory.h"
#include "ItemCatalog.h"

Inventory::Inventory()
{
	m_font.loadFromFile("data/arial.ttf");

	sf::Texture t;
	bool r = t.create(400, 400);
	m_window.setTexture(t);
	m_window.setColor(sf::Color::Red);
}

bool Inventory::IsOpen()
{
	return m_isOpen;
}

void Inventory::Display()
{
	m_isOpen = !m_isOpen;
}

//void Inventory::AddItem(string name, int quantity)
//{
//	string newDesc;
//	if (ItemCatalog::GetItemDescription(name, newDesc))
//	{
//		sf::Text newText;
//
//		newText.setCharacterSize(12);
//		newText.setColor(sf::Color::Black);
//		newText.setFont(m_font);
//		newText.setString(name + '\n' + newDesc);
//
//		Item item;
//		ItemCatalog::GetItem(name, item);
//		sf::Texture itemIcon = item.GetTexture();
//		InventoryItem* newItem = new InventoryItem(itemIcon, newText);
//
//		newItem->SetPosition(sf::Vector2f(0, m_heldItems.size()*36.f));
//		
//		map<InventoryItem*, int>::iterator query;
//		query = m_heldItems.find(newItem);
//		if (query == m_heldItems.end())
//		{
//			m_heldItems.insert(std::make_pair(newItem, quantity));
//		
//		}
//		else
//		{
//			m_heldItems[newItem] += quantity;
//		}
//	}
//	
//}

void Inventory::Draw(sf::RenderWindow& rw)
{
	if (m_isOpen)
	{
		rw.draw(m_window);
		for (map<InventoryItem*, int>::iterator i = m_heldItems.begin(); i != m_heldItems.end(); i++)
		{
			i->first->Draw(rw);
		}
	}
		
}
