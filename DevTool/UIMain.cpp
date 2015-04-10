#include "UIMain.h"


UIMain::UIMain()
{
	m_guiWindow = sfg::Window::Create();
	m_notebook = sfg::Notebook::Create();

	m_guiWindow->SetTitle("Title");

	m_componentList.LoadComponents(COMPONENT_LIST_FILE);

	ItemMap items;
	m_itemList.SetComponentList(&m_componentList);
	m_itemList.LoadItems(ITEM_LIST_FILE);
	m_itemList.GetItems(items);

	std::map<Slots::SlotName, vector<string>> itemsBySlot;
	m_itemList.GetItemsBySlot(itemsBySlot);
	m_entityList.SetComponentList(&m_componentList);
	m_entityList.LoadEntities(ENTITY_LIST_FILE);

	ComponentMap compMap;
	m_componentList.GetComponents(compMap);
	m_itemPageUI.SetItemList(&m_itemList);
	m_itemPageUI.PopulateComboBox();
	m_itemPageUI.SetItemComponents(compMap);

	m_entityPageUI.SetItems(itemsBySlot);
	m_entityPageUI.SetEntityList(&m_entityList);
	m_entityPageUI.PopulateComboBox();
	m_entityPageUI.SetEntityComponents(compMap);

	m_componentPageUI.SetComponentList(&m_componentList);
	m_componentPageUI.PopulateComboBox();

	m_notebook->AppendPage(m_entityPageUI.GetPage(), sfg::Label::Create("Entities"));
	m_notebook->AppendPage(m_itemPageUI.GetPage(), sfg::Label::Create("Items"));
	m_notebook->AppendPage(m_componentPageUI.GetPage(), sfg::Label::Create("Components"));

	m_notebook->SetRequisition(sf::Vector2f(400.f, 0.f));
	m_notebook->SetScrollable(true);	

	m_guiWindow->Add(m_notebook);
}


UIMain::~UIMain()
{
}

void UIMain::HandleEvent(sf::Event event)
{
	m_guiWindow->HandleEvent(event);
}

void UIMain::Update(float dt)
{
	m_guiWindow->Update(dt);
	m_componentPageUI.Update(dt);
	m_itemPageUI.Update(dt);
	m_entityPageUI.Update(dt);
}

void UIMain::Draw(sf::RenderWindow& rw)
{
	sfgui.Display(rw);
}
