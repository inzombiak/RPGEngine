#ifndef UI_MAIN_H
#define UI_MAIN_H

#include "ComponentPageUI.h"

class UIMain
{
public:
	UIMain();
	~UIMain();

private:
	const string COMPONENT_LIST_FILE = "../SFMLGame2/data/ComponentList.xml";
	const string ENTITY_LIST_FILE = "../SFMLGame2/data/Entities.xml";
	const string ITEM_LIST_FILE = "../SFMLGame2/data/MainItemCatalog.xml";

	sfg::SFGUI sfgui;
	sfg::Window::Ptr m_guiWindow;
	sfg::Notebook::Ptr m_notebook;

};

#endif
