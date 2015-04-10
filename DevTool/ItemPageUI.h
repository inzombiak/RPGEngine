#ifndef ITEM_PAGE_UI_H
#define ITEM_PAGE_UI_H

#include "ItemList.h"
#include "SFGUI\SFGUI.hpp"

class ItemPageUI
{
public:
	ItemPageUI();
	sfg::Box::Ptr GetPage()
	{
		return m_itemMainBox;
	}
	void SetItemList(ItemList* il)
	{
		m_itemList = il;
	}
	void Update(float dt)
	{
		while (!m_deleteList.empty())
		{
			m_deleteList.back()->RemoveAll();
			m_deleteList.pop_back();
		}
	}
	void PopulateComboBox();
	void SetItemComponents(ComponentMap components)
	{
		for (ComponentMap::iterator it = components.begin(); it != components.end(); it++)
			if (it->second.isItemComponent)
				m_components[it->first] = it->second;
	}
private:
	void GenerateForm(Item&);
	sfg::Box::Ptr GenerateComponentForm(Component&);
	void AddFormToPage(){};
	void ClearForm();

	void OnAddComponent();
	void OnAddListEntry(sfg::Box::Ptr target, int compID, vector<Field> entryFields);
	void OnDeleteComponent(sfg::Box::Ptr target, int compID); //Adds the component contianer to delete list and removes fields from m_fieldList;
	void OnDeleteListEntry(sfg::Box::Ptr target, int compID, int entryID); //Adds list entry container to delete list and removes from m_listEntryList;
	void OnItemSelect();
	void OnSaveItem();
	void OnCreateNewItem();

	sfg::Box::Ptr m_itemMainBox;
	sfg::Box::Ptr m_itemSelectBox;
	sfg::Box::Ptr m_itemEditBox;
	sfg::Box::Ptr m_addCompBox;
	sfg::ComboBox::Ptr m_itemComboBox;
	sfg::ComboBox::Ptr m_itemCompComboBox;
	sfg::Button::Ptr m_createNewItemButton;
	sfg::Button::Ptr m_addComponentButton;
	sfg::Button::Ptr m_saveItemButton;
	vector<sfg::Box::Ptr> m_deleteList;
	vector<sfg::Box::Ptr> m_children;
	//Find a better way of storing component entries
	vector<string> m_componentNames;
	std::map<int, vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>>> m_fieldList;
	std::map<int, std::map<int, vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>>>> m_listEntryList;

	sfg::Image::Ptr m_deleteIcon;
	sf::Image m_deleteImageHodler;

	ItemList* m_itemList;
	ComponentMap m_components;
};

#endif

