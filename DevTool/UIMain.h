#ifndef UI_MAIN_H
#define UI_MAIN_H

#include "ComponentPageUI.h"
#include "ItemPageUI.h"
#include "ItemList.h"
#include "EntityList.h"
#include "EntityPageUI.h"

class UIMain
{
public:
	UIMain();
	~UIMain();

	void Draw(sf::RenderWindow& rw);
	void HandleEvent(sf::Event);
	void Update(float dt);

private:
	const string COMPONENT_LIST_FILE = "testComponent.xml";
	const string ENTITY_LIST_FILE = "testEntities.xml";
	const string ITEM_LIST_FILE = "testItem.xml";

	ComponentList m_componentList;
	ItemList m_itemList;
	EntityList m_entityList;

	ComponentPageUI m_componentPageUI;
	ItemPageUI m_itemPageUI;
	EntityPageUI m_entityPageUI;

	sfg::SFGUI sfgui;
	sfg::Window::Ptr m_guiWindow;
	sfg::Notebook::Ptr m_notebook;

};

#endif
