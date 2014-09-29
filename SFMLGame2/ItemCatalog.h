//#include "Item.h"
//
//#include <map>
//#include <string>
//#include <fstream>
//#include <iostream>
//
//using std::map;
//using std::fstream;
//using std::string;
//using std::iostream;
//using std::ifstream;
//
//class ItemCatalog
//{
//public:
//	static void Initialize(string);
//	
//	//static bool GetItem(string, Item&);
//	//static void AddItem(Item&);
//	static bool GetItemDescription(string, string&);
//	static void Destroy();
//
//	static ItemCatalog* GetInstance()
//	{
//		return m_instance;
//	}
//
//private:
//	ItemCatalog();
//	static ItemCatalog* m_instance;
//	static void CreateItem(string, string, string);
//	static void ReadItems(string);
//	static bool m_initialized;
////	static map<string, Item*> m_itemCatalog;
//};