#ifndef Entity_PAGE_UI_H
#define Entity_PAGE_UI_H

#include "EntityList.h"
#include "SFGUI\SFGUI.hpp"

class EntityPageUI
{
public:
	EntityPageUI();
	sfg::Box::Ptr GetPage()
	{
		return m_entityMainBox;
	}
	void SetEntityList(EntityList* il)
	{
		m_entityList = il;
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
	void SetEntityComponents(ComponentMap components)
	{
		for (ComponentMap::iterator it = components.begin(); it != components.end(); it++)
			if (!it->second.isItemComponent)
				m_components[it->first] = it->second;
	}
	void SetItems(std::map<Slots::SlotName, vector<string>> items)
	{
		m_items = items;
		
	}
private:
	void GenerateForm(Entity&);
	sfg::Box::Ptr GenerateComponentForm(Component&);
	void AddFormToPage(){};
	void ClearForm();

	void OnAddComponent();
	void OnAddListEntry(sfg::Box::Ptr target, int compID, vector<Field> entryFields);
	void OnDeleteComponent(sfg::Box::Ptr target, int compID); //Adds the component contianer to delete list and removes fields from m_fieldList;
	void OnDeleteListEntry(sfg::Box::Ptr target, int compID, int entryID); //Adds list entry container to delete list and removes from m_listEntryList;
	void OnEntitySelect();
	void OnSaveEntity();
	void OnCreateNewEntity();
	void OnEquipmentSlotChange(int compID, int mapID, int entryID);


	sfg::Box::Ptr m_entityMainBox;
	sfg::Box::Ptr m_entitySelectBox;
	sfg::Box::Ptr m_entityEditBox;
	sfg::Box::Ptr m_addCompBox;
	sfg::ComboBox::Ptr m_entityComboBox;
	sfg::ComboBox::Ptr m_entityCompComboBox;
	sfg::Button::Ptr m_createNewEntityButton;
	sfg::Button::Ptr m_addComponentButton;
	sfg::Button::Ptr m_saveEntityButton;
	sfg::ScrolledWindow::Ptr m_scrolledWindow;
	vector<sfg::Box::Ptr> m_deleteList;
	vector<sfg::Box::Ptr> m_children;
	
	//Form input widgets
	sfg::Entry::Ptr m_entityNameEntry;
	vector<string> m_componentNames;
	std::map<int, vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>>> m_fieldList;
	std::map<int, std::map<int, vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>>>> m_listEntryList;

	sfg::Image::Ptr m_deleteIcon;
	sf::Image m_deleteImageHodler;

	int m_selectedEntityID; //Contains id of item that was selected or -1 if a new item was created

	EntityList* m_entityList;
	ComponentMap m_components;
	std::map<Slots::SlotName, vector<string>> m_items;
};

#endif
