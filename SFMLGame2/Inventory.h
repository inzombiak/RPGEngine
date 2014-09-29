#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include "InventoryItem.h"
#include <vector>
#include <map>
#include <iostream>
#include <string>

using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::string;

class Inventory
{
public:
	Inventory();
	//void AddItem(string, int);
	void Display();
	bool IsOpen();
	void Draw(sf::RenderWindow&);

private:
	class SFMLTextComparator
	{
	public:
		bool operator()(const InventoryItem* x, const InventoryItem* y) { return x->GetText().getString() > y->GetText().getString(); }
	};
	bool m_isOpen;
	sf::Font m_font;
	sf::Sprite m_window;
	map<InventoryItem*, int, SFMLTextComparator> m_heldItems;
};

#endif