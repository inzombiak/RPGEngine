#include "ItemPageUI.h"


ItemPageUI::ItemPageUI()
{
	m_itemMainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_itemSelectBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	m_itemEditBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_addComponentButton = sfg::Button::Create("AddComponent");
	m_saveItemButton = sfg::Button::Create("Save");
	m_createNewItemButton = sfg::Button::Create("New Item");
	m_itemCompComboBox = sfg::ComboBox::Create();
	m_itemComboBox = sfg::ComboBox::Create();

	m_itemComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&ItemPageUI::OnItemSelect, this));
	m_addComponentButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ItemPageUI::OnAddComponent, this));
	m_saveItemButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ItemPageUI::OnSaveItem, this));
	m_createNewItemButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ItemPageUI::OnCreateNewItem, this));
	
	auto comboText = sfg::Label::Create();
	comboText->SetText("Select an Item to edit or click New to create a new item");

	sf::Image m_deleteImageHodler;
	m_deleteImageHodler.loadFromFile("icon_delete.png");
	m_deleteIcon = sfg::Image::Create(m_deleteImageHodler);

	m_itemSelectBox->Pack(comboText);
	m_itemSelectBox->Pack(m_itemComboBox, false);
	m_itemMainBox->Pack(m_itemSelectBox, false);
	m_itemMainBox->Pack(m_createNewItemButton, false);
	m_itemMainBox->Pack(m_itemEditBox);

	m_addCompBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	m_addCompBox->Pack(m_addComponentButton);
	m_addCompBox->Pack(m_itemCompComboBox);
	m_addCompBox->Show(false);
	m_itemMainBox->Pack(m_addCompBox, false);
	m_saveItemButton->Show(false);
	m_itemMainBox->Pack(m_saveItemButton, false);
}

void ItemPageUI::PopulateComboBox()
{
	ItemMap items;
	m_itemList->GetItems(items);

	for (ItemMap::iterator i = items.begin(); i != items.end(); ++i)
	{
		m_itemComboBox->AppendItem(i->second.name);
	}
}

void ItemPageUI::ClearForm()
{
	m_componentNames.clear();
	m_addCompBox->Show(false);

	while (m_itemCompComboBox->GetItemCount() > 0)
		m_itemCompComboBox->RemoveItem(0);

	for (auto it = m_children.begin(); it != m_children.end(); ++it)
		(*it)->RemoveAll();
	m_fieldList.clear();

	m_listEntryList.clear();

	m_itemEditBox->RemoveAll();
}

void ItemPageUI::GenerateForm(Item& item)
{
	ClearForm();
	vector<string> itemCompNames;
	for(int i = 0; i < item.components.size(); ++i)
	{
		m_itemEditBox->Pack(GenerateComponentForm(item.components[i]), false);
		itemCompNames.push_back(item.components[i].name);
	}
	vector<string>::iterator search;
	for (ComponentMap::iterator it = m_components.begin(); it != m_components.end(); it++)
	{
		search = std::find(itemCompNames.begin(), itemCompNames.end(), it->second.name);
		if (search == itemCompNames.end())
			m_itemCompComboBox->AppendItem(it->second.name);
		else
			itemCompNames.erase(search);
	}

	m_addCompBox->Show();
	m_saveItemButton->Show();
}

sfg::Box::Ptr ItemPageUI::GenerateComponentForm(Component& comp)
{
	int compID = reinterpret_cast<int>(HashedString::hash_name(comp.name.c_str()));

	sfg::Box::Ptr result = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	sfg::Box::Ptr compBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	sfg::Label::Ptr compNameLbl = sfg::Label::Create("Name");
	sfg::Label::Ptr compName = sfg::Label::Create(comp.name);

	auto deleteButton = sfg::Button::Create("Delete");
	deleteButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ItemPageUI::OnDeleteComponent, this, result, compID));
	
	compBox->Pack(compNameLbl);
	compBox->Pack(compName);
	compBox->Pack(deleteButton);
	m_children.push_back(compBox);
	result->Pack(compBox, false);
	m_componentNames.push_back(comp.name);
	vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>> compFieldEntries;

	for (int i = 0; i < comp.fields.size(); ++i)
	{
		sfg::Box::Ptr compFieldBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
		sfg::Label::Ptr compFieldLbl = sfg::Label::Create(comp.fields[i].name);
		compFieldBox->Pack(compFieldLbl);

		auto newEntry = UtilityFormFunctions::AddComponentWidgetToBox(comp.fields[i].valueType, comp.fields[i].value, compFieldBox);
		compFieldEntries.push_back(std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>(compFieldLbl, newEntry));
		
		result->Pack(compFieldBox, false);
		m_children.push_back(compFieldBox);
	}
	
	m_fieldList[compID] = (compFieldEntries);
	if (comp.lists.size() == 0)
		return result;

	sfg::Label::Ptr listLabel = sfg::Label::Create(comp.lists[0].name);
	result->Pack(listLabel);

	for (int i = 0; i < comp.lists.size(); i++)
	{
		sfg::Box::Ptr compListBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

		std::map<int, vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>>> compListEntries;
		vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>> listDef;
		listDef.push_back(std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>(listLabel, sfg::Label::Create(comp.lists[i].entryName)));

		compListEntries[0] = listDef;
		int entryIndex = 1;
		for (int k = 0; k < comp.lists[i].entries.size(); ++k)
		{
			vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>> compListEntryFields;
			ListEntry currEntry = comp.lists[i].entries[k];
			sfg::Box::Ptr compListEntryBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
			auto deleteListEntryButton = sfg::Button::Create("Delete");
			deleteListEntryButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ItemPageUI::OnDeleteListEntry, this, compListEntryBox, compID, entryIndex));

			for (int j = 0; j < currEntry.fields.size(); ++j)
			{
				sfg::Label::Ptr compFieldLbl = sfg::Label::Create(currEntry.fields[j].name);
				compListEntryBox->Pack(compFieldLbl);
				auto newEntry = UtilityFormFunctions::AddComponentWidgetToBox(currEntry.fields[j].valueType, currEntry.fields[j].value, compListEntryBox);
				compListEntryFields.push_back(std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>(compFieldLbl, newEntry));
				m_children.push_back(compListEntryBox);
			}

			compListEntryBox->Pack(deleteListEntryButton);
			compListBox->Pack(compListEntryBox);
			compListEntries[entryIndex] = compListEntryFields;
			m_children.push_back(compListBox);
			entryIndex++;
		}
		sfg::Button::Ptr addEntryBtn = sfg::Button::Create("Add Entry");
		addEntryBtn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ItemPageUI::OnAddListEntry, this, compListBox, compID, comp.lists[i].entries[0].fields));

		result->Pack(compListBox, false);
		result->Pack(addEntryBtn, false);
		m_listEntryList[compID] = compListEntries;
	}
	
	return result;
}

void ItemPageUI::OnItemSelect()
{
	string compName = m_itemComboBox->GetSelectedText();
	int id = reinterpret_cast<int>(HashedString::hash_name(compName.c_str()));
	Item selectedItem;
	if (!m_itemList->GetItemByID(id, selectedItem))
		return;
	GenerateForm(selectedItem);
}

void ItemPageUI::OnAddComponent()
{
	string compName = m_itemCompComboBox->GetSelectedText();
	if (compName.size() == 0)
		return;
	Component newComp = m_components[reinterpret_cast<int>(HashedString::hash_name(compName.c_str()))];
	m_itemEditBox->Pack(GenerateComponentForm(newComp));
	m_itemCompComboBox->RemoveItem(m_itemCompComboBox->GetSelectedItem());
}

void ItemPageUI::OnAddListEntry(sfg::Box::Ptr target, int compID, vector<Field> entryFields)
{
	sfg::Box::Ptr compListBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>> entry;
	for (int j = 0; j < entryFields.size(); ++j)
	{
		sfg::Label::Ptr listEntryFieldLbl = sfg::Label::Create(entryFields[j].name);
		compListBox->Pack(listEntryFieldLbl);
		auto listEntryFieldValue =  UtilityFormFunctions::AddComponentWidgetToBox(entryFields[j].valueType, entryFields[j].value, compListBox);
		entry.push_back(std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>(listEntryFieldLbl, listEntryFieldValue));
	}
	int entryIndex = m_listEntryList[compID].rbegin()->first + 1;
	m_listEntryList[compID][entryIndex] = entry;
	auto deleteListEntryButton = sfg::Button::Create("Delete");
	deleteListEntryButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ItemPageUI::OnDeleteListEntry, this, compListBox, compID, entryIndex));

	compListBox->Pack(deleteListEntryButton);
	m_children.push_back(compListBox);
	target->Pack(compListBox);
}

void ItemPageUI::OnSaveItem()
{
	Item item;
	auto nameIter = m_fieldList.find(reinterpret_cast<int>(HashedString::hash_name("BaseItemComponent")));
	item.name = UtilityFormFunctions::GetTextFromWidget(nameIter->second[0].second);
	
	for (int i = 0; i < m_componentNames.size(); ++i)
	{
		Component newComp;
		newComp.name = m_componentNames[i];
		int compID = reinterpret_cast<int>(HashedString::hash_name(newComp.name.c_str()));
		auto fieldIter = m_fieldList.find(compID);
		for (int j = 0; j < fieldIter->second.size(); ++j)
		{
			Field newField;
			newField.name = fieldIter->second[j].first->GetText();
			newField.value = UtilityFormFunctions::GetTextFromWidget(fieldIter->second[j].second);
			newComp.fields.push_back(newField);
		}

		auto listIter = m_listEntryList.find(compID);
		if (listIter != m_listEntryList.end())
		{
			auto compList = listIter->second;
			List newList;
			newList.name = compList[0][0].first->GetText();
			newList.entryName = UtilityFormFunctions::GetTextFromWidget(compList[0][0].second);
			auto it = compList.begin();
			it++;
			for (; it != compList.end(); ++it)
			{
				ListEntry newListEntry;
				for (int j = 0; j < it->second.size(); ++j)
				{
					Field entryField;
					entryField.name = it->second[j].first->GetText();
					entryField.value = UtilityFormFunctions::GetTextFromWidget(it->second[j].second);
					newListEntry.fields.push_back(entryField);
				}
				newList.entries.push_back(newListEntry);
			}
			newComp.lists.push_back(newList);
		}
		item.components.push_back(newComp);
	}

	m_itemList->SetItemByID(reinterpret_cast<int>(HashedString::hash_name(item.name.c_str())), item);
	
	while (m_itemComboBox->GetItemCount() > 0)
		m_itemComboBox->RemoveItem(0);
	ClearForm();
	PopulateComboBox();
	m_saveItemButton->Show(true);
}

void ItemPageUI::OnDeleteComponent(sfg::Box::Ptr target, int id)
{
	m_deleteList.push_back(target);
	auto it = m_fieldList.find(id);
	if (it != m_fieldList.end())
		m_fieldList.erase(it);
}

void ItemPageUI::OnDeleteListEntry(sfg::Box::Ptr target, int id, int entryID)
{
	m_deleteList.push_back(target);
	auto it = m_listEntryList.find(id);
	if (it != m_listEntryList.end())
	{
		auto entryIter = (*it).second.find(entryID);
		if (entryIter != (*it).second.end())
			(*it).second.erase(entryIter);
	}
		
}

void ItemPageUI::OnCreateNewItem()
{
	Item item;
	GenerateForm(item);
}