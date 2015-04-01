#ifndef ComponentPageUI_H
#define ComponentPageUI_H

#include "ComponentList.h"
#include "EntityList.h"
#include "ItemList.h"
#include "SFGUI/SFGUI.hpp"

class ComponentPageUI
{
public:
	ComponentPageUI();
	sfg::Box::Ptr GetPage();
	void Draw(sf::RenderWindow& rw);
	void HandleEvent(sf::Event);
	void Update(float dt);
	bool LoadList(string filepath);

private:
	void PopulateTable();
	void GenerateForm(Component&);
	void AddFormToPage();

	void OnComponentComboSelect();
	void OnAddNewComponent();
	void OnSaveComponentClick();
	void OnAddFieldClick();
	void OnAddListClick();
	void OnAddListFieldClick();

	bool m_clearForm = false;
	bool m_updateForm = false;

	sfg::Box::Ptr m_componentMainBox;
	sfg::Box::Ptr m_componentSelectBox;
	sfg::Box::Ptr m_componentEditBox;
	sfg::Table::Ptr m_fieldTable;
	sfg::Table::Ptr m_listTable;
	sfg::Button::Ptr m_addNewComponent;
	sfg::Button::Ptr m_saveComponentBtn;
	sfg::Button::Ptr m_addFieldBtn;
	sfg::Button::Ptr m_addListBtn;
	sfg::Button::Ptr m_addListFieldButton;
	sfg::CheckButton::Ptr m_isItemComponentCheckBtn;
	sfg::ComboBox::Ptr m_componentListCombo;

	sfg::Entry::Ptr m_componentNameEntry;
	vector<std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>> m_fieldEntries;
	vector<std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>> m_listEntries;
	vector<vector<std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>>> m_listFieldEntries;

	ComponentList m_componentList;
	EntityList m_entityList;
	ItemList m_itemList;
};

#endif
