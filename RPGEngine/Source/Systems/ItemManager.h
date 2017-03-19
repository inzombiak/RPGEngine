#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

/*
TODO
Merge Item and ItemComponent creation with Entity nad EntityComponent creation
*/

#include <string>
#include <map>
#include <vector>
#include "..\Utilities\GameDefs.h"
#include "..\Utilities\tinyxml2.h"
#include "..\Events\IEventData.h"
#include "..\Components\ItemComponent.h"
#include "..\Components\InventoryComponent.h"

using std::map;
using std::string;
using std::vector;

using tinyxml2::XMLElement;
class EntityManager;
class ItemManager
{
public:
	ItemManager();
	void LoadItemCatalog(string filepath);
	void SetEntityManager(EntityManager* em)
	{
		m_entityManager = em;
	}
	static StrongComponentPtr CreateInventoryComponent();
	static StrongComponentPtr CreateItemPickupComponent();
	static StrongComponentPtr CreateEquipmentComponent();
	static bool CreateItemByID(ItemID itemID, Item& item);
	static bool DropItem(ItemID, sf::Vector2f);
private:
	//Stores information for creating an item instance, not actual items TODO: Change from StrongItemCompoenentPtr ot XMLElement*?
	class ItemDefinition
	{
	public:
		string name;
		vector<std::pair<ComponentID, XMLElement*>> itemComponents;
	};
	typedef map<ItemID, ItemDefinition> ItemCatalog;
	tinyxml2::XMLDocument m_xmlFile;

	void EquipItem(IEventData* data);

	bool InitializeItem(XMLElement* node, ItemDefinition& item);

	StrongItemComponentPtr CreateItemComponent(XMLElement* node);

	static EntityManager* m_entityManager; //For creating dropped items

	static ItemCatalog m_itemCatalog;

	static std::vector<std::shared_ptr<InventoryComponent>> m_inventoryComponents;

};
#endif