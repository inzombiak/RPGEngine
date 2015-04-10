#include "EntityPageUI.h"

EntityPageUI::EntityPageUI()
{
	m_entityMainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_entitySelectBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	m_entityEditBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_addComponentButton = sfg::Button::Create("AddComponent");
	m_saveEntityButton = sfg::Button::Create("Save");
	m_createNewEntityButton = sfg::Button::Create("New Entity");
	m_entityCompComboBox = sfg::ComboBox::Create();
	m_entityComboBox = sfg::ComboBox::Create();
	m_entityNameEntry = sfg::Entry::Create();
	m_scrolledWindow = sfg::ScrolledWindow::Create();

	m_scrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
	m_scrolledWindow->AddWithViewport(m_entityEditBox);
	m_scrolledWindow->SetRequisition(sf::Vector2f(.0f, 400.f));

	m_entityComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&EntityPageUI::OnEntitySelect, this));
	m_addComponentButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&EntityPageUI::OnAddComponent, this));
	m_saveEntityButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&EntityPageUI::OnSaveEntity, this));
	m_createNewEntityButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&EntityPageUI::OnCreateNewEntity, this));

	m_entityEditBox->SetRequisition(sf::Vector2f(600.f, 300.f));

	auto comboText = sfg::Label::Create();
	comboText->SetText("Select an Entity to edit or click New to create a new entity");

	sf::Image m_deleteImageHodler;
	m_deleteImageHodler.loadFromFile("icon_delete.png");
	m_deleteIcon = sfg::Image::Create(m_deleteImageHodler);

	m_entitySelectBox->Pack(comboText);
	m_entitySelectBox->Pack(m_entityComboBox, false);
	m_entityMainBox->Pack(m_entitySelectBox, false);
	m_entityMainBox->Pack(m_createNewEntityButton, false);
	m_entityMainBox->Pack(m_scrolledWindow, false);

	m_addCompBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	m_addCompBox->Pack(m_addComponentButton);
	m_addCompBox->Pack(m_entityCompComboBox);
	m_addCompBox->Show(false);
	m_entityMainBox->Pack(m_addCompBox, false);
	m_saveEntityButton->Show(false);
	m_entityMainBox->Pack(m_saveEntityButton, false);
}

void EntityPageUI::PopulateComboBox()
{
	EntityMap entitys;
	m_entityList->GetEntities(entitys);

	for (EntityMap::iterator i = entitys.begin(); i != entitys.end(); ++i)
	{
		m_entityComboBox->AppendItem(i->second.name);
	}
}

void EntityPageUI::ClearForm()
{
	m_componentNames.clear();
	m_addCompBox->Show(false);

	while (m_entityCompComboBox->GetItemCount() > 0)
		m_entityCompComboBox->RemoveItem(0);

	for (auto it = m_children.begin(); it != m_children.end(); ++it)
		(*it)->RemoveAll();
	m_fieldList.clear();

	m_listEntryList.clear();

	m_entityEditBox->RemoveAll();
}

void EntityPageUI::GenerateForm(Entity& entity)
{
	ClearForm();
	auto entityNameLbl = sfg::Label::Create("Entity Name");
	m_entityNameEntry->SetText(entity.name.c_str());

	auto nameBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	nameBox->Pack(entityNameLbl);
	nameBox->Pack(m_entityNameEntry);
	m_entityEditBox->Pack(nameBox);
	m_children.push_back(nameBox);

	vector<string> entityCompNames;
	for (int i = 0; i < entity.components.size(); ++i)
	{
		m_entityEditBox->Pack(GenerateComponentForm(entity.components[i]), false);
		entityCompNames.push_back(entity.components[i].name);
	}
	vector<string>::iterator search;
	for (ComponentMap::iterator it = m_components.begin(); it != m_components.end(); it++)
	{
		search = std::find(entityCompNames.begin(), entityCompNames.end(), it->second.name);
		if (search == entityCompNames.end())
			m_entityCompComboBox->AppendItem(it->second.name);
		else
			entityCompNames.erase(search);
	}

	m_addCompBox->Show();
	m_saveEntityButton->Show();
}

sfg::Box::Ptr EntityPageUI::GenerateComponentForm(Component& comp)
{
	int compID = reinterpret_cast<int>(HashedString::hash_name(comp.name.c_str()));

	sfg::Box::Ptr result = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
	sfg::Box::Ptr compBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
	sfg::Label::Ptr compNameLbl = sfg::Label::Create("Name");
	sfg::Label::Ptr compName = sfg::Label::Create(comp.name);

	auto deleteButton = sfg::Button::Create("Delete");
	deleteButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&EntityPageUI::OnDeleteComponent, this, result, compID));

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
		sfg::Box::Ptr compListBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);

		std::map<int, vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>>> compListEntries;
		vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>> listDef;
		listDef.push_back(std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>(listLabel, sfg::Label::Create(comp.lists[i].entryName)));

		compListEntries[0] = listDef;
		int entryIndex = 1;

		/*
		Temporary until I can find a less hard coded system
		*/
		vector<string> values;
		if (comp.name == "InventoryComponent")
		{
			for (auto it = m_items.begin(); it != m_items.end(); ++it)
			{
				for (int index = 0; index < it->second.size(); ++index)
				{
					values.push_back(it->second[index]);
				}
			}
		}
		for (int k = 0; k < comp.lists[i].entries.size(); ++k)
		{
			vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>> compListEntryFields;
			ListEntry currEntry = comp.lists[i].entries[k];
			sfg::Box::Ptr compListEntryBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
			auto deleteListEntryButton = sfg::Button::Create("Delete");
			deleteListEntryButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&EntityPageUI::OnDeleteListEntry, this, compListEntryBox, compID, entryIndex));

			for (int j = 0; j < currEntry.fields.size(); ++j)
			{
				sfg::Label::Ptr compFieldLbl = sfg::Label::Create(currEntry.fields[j].name);
				compListEntryBox->Pack(compFieldLbl);

				//Streamline this process
				if (comp.name.compare("EquipmentComponent") == 0 && currEntry.fields[j].name.compare("itemname") == 0)
					values = m_items[Slots::slotStringtoEnumMap.find(currEntry.fields[j-1].value)->second];
				auto newEntry = UtilityFormFunctions::AddComponentWidgetToBox(currEntry.fields[j].valueType, currEntry.fields[j].value, compListEntryBox, values);
				if (comp.name.compare("EquipmentComponent") == 0 && currEntry.fields[j].name.compare("name") == 0)
					newEntry->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&EntityPageUI::OnEquipmentSlotChange, this, compID, entryIndex, j));

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
		addEntryBtn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&EntityPageUI::OnAddListEntry, this, compListBox, compID, comp.lists[i].entries[0].fields));

		result->Pack(compListBox, false);
		result->Pack(addEntryBtn, false);
		m_listEntryList[compID] = compListEntries;
	}

	return result;
}

void EntityPageUI::OnEntitySelect()
{
	string compName = m_entityComboBox->GetSelectedText();
	int id = reinterpret_cast<int>(HashedString::hash_name(compName.c_str()));
	Entity selectedEntity;
	if (!m_entityList->GetEntityByID(id, selectedEntity))
		return;
	m_selectedEntityID = id;
	GenerateForm(selectedEntity);
}

void EntityPageUI::OnAddComponent()
{
	string compName = m_entityCompComboBox->GetSelectedText();
	if (compName.size() == 0)
		return;
	Component newComp = m_components[reinterpret_cast<int>(HashedString::hash_name(compName.c_str()))];
	m_entityEditBox->Pack(GenerateComponentForm(newComp));
	m_entityCompComboBox->RemoveItem(m_entityCompComboBox->GetSelectedItem());
}

void EntityPageUI::OnAddListEntry(sfg::Box::Ptr target, int compID, vector<Field> entryFields)
{
	vector<string> values;
	if (compID == reinterpret_cast<int>(HashedString::hash_name("InventoryComponent")))
	{
		for (auto it = m_items.begin(); it != m_items.end(); ++it)
		{
			for (int index = 0; index < it->second.size(); ++index)
			{
				values.push_back(it->second[index]);
			}
		}
	}

	sfg::Box::Ptr compListBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	vector<std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>> entry;
	for (int j = 0; j < entryFields.size(); ++j)
	{
		sfg::Label::Ptr listEntryFieldLbl = sfg::Label::Create(entryFields[j].name);
		compListBox->Pack(listEntryFieldLbl);
		auto listEntryFieldValue = UtilityFormFunctions::AddComponentWidgetToBox(entryFields[j].valueType, entryFields[j].value, compListBox, values);
		entry.push_back(std::pair<sfg::Label::Ptr, sfg::Widget::Ptr>(listEntryFieldLbl, listEntryFieldValue));
	}

	int entryIndex = m_listEntryList[compID].rbegin()->first + 1;
	m_listEntryList[compID][entryIndex] = entry;
	auto deleteListEntryButton = sfg::Button::Create("Delete");
	deleteListEntryButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&EntityPageUI::OnDeleteListEntry, this, compListBox, compID, entryIndex));

	compListBox->Pack(deleteListEntryButton);
	m_children.push_back(compListBox);
	target->Pack(compListBox);
}

void EntityPageUI::OnSaveEntity()
{
	Entity entity;
	entity.name = m_entityNameEntry->GetText();

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
		entity.components.push_back(newComp);
	}

	if (m_selectedEntityID == -1)
		m_selectedEntityID = reinterpret_cast<int>(HashedString::hash_name(entity.name.c_str()));
	m_entityList->SetEntityByID(m_selectedEntityID, entity);

	while (m_entityComboBox->GetItemCount() > 0)
		m_entityComboBox->RemoveItem(0);
	ClearForm();
	PopulateComboBox();
	m_saveEntityButton->Show(true);
}

void EntityPageUI::OnDeleteComponent(sfg::Box::Ptr target, int id)
{
	m_deleteList.push_back(target);
	auto it = m_fieldList.find(id);
	if (it != m_fieldList.end())
		m_fieldList.erase(it);
}

void EntityPageUI::OnDeleteListEntry(sfg::Box::Ptr target, int id, int entryID)
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

void EntityPageUI::OnCreateNewEntity()
{
	Entity entity;
	GenerateForm(entity);
	m_selectedEntityID = -1;
}

void EntityPageUI::OnEquipmentSlotChange(int compID, int mapID, int entryID)
{
	/*
	Streamline
	*/

	auto list = m_listEntryList[compID];
	auto entry = list[mapID];
	auto slotSelectBox = entry[entryID].second;

	string selectedSlotName = UtilityFormFunctions::GetTextFromWidget(slotSelectBox);
	sfg::ComboBox::Ptr  itemSelectBox = std::dynamic_pointer_cast<sfg::ComboBox, sfg::Widget>(entry[entryID+1].second);
	while (itemSelectBox->GetItemCount() > 0)
		itemSelectBox->RemoveItem(0);
	vector<string> itemNames = m_items[Slots::slotStringtoEnumMap.find(selectedSlotName)->second];

	for (int i = 0; i < itemNames.size(); ++i)
		itemSelectBox->AppendItem(itemNames[i]);
}