#include "ComponentPageUI.h"

ComponentPageUI::ComponentPageUI()
{
	m_componentMainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5);
	m_componentSelectBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5);
	m_componentEditBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5);
	m_fieldTable = sfg::Table::Create();
	m_listTable = sfg::Table::Create();
	m_saveComponentBtn = sfg::Button::Create();
	m_addNewComponent = sfg::Button::Create();
	m_addFieldBtn = sfg::Button::Create();
	m_addListBtn = sfg::Button::Create();
	m_addListFieldButton = sfg::Button::Create();
	m_isItemComponentCheckBtn = sfg::CheckButton::Create("Is Item Component?");
	m_componentListCombo = sfg::ComboBox::Create();
	
	auto comboText = sfg::Label::Create();
	comboText->SetText("Select a Component to edit or click New to create a new component");
	m_componentListCombo->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&ComponentPageUI::OnComponentComboSelect, this));
	m_saveComponentBtn->SetLabel("Save");
	m_saveComponentBtn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ComponentPageUI::OnSaveComponentClick, this));
	m_addNewComponent->SetLabel("Add New Component");
	m_addNewComponent->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ComponentPageUI::OnAddNewComponent, this));
	m_addFieldBtn->SetLabel("Add Field");
	m_addFieldBtn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ComponentPageUI::OnAddFieldClick, this));
	m_addListBtn->SetLabel("Add List");
	m_addListBtn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ComponentPageUI::OnAddListClick, this));
	m_addListFieldButton->SetLabel("Add List Field");
	m_addListFieldButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ComponentPageUI::OnAddListFieldClick, this));

	m_componentSelectBox->Pack(comboText);
	m_componentSelectBox->Pack(m_componentListCombo, false);
}

sfg::Box::Ptr ComponentPageUI::GetPage()
{
	return m_componentMainBox;
}

void ComponentPageUI::HandleEvent(sf::Event event)
{
	m_guiWindow->HandleEvent(event);
	if (m_clearForm)
	{
		m_componentEditBox->RemoveAll();
		m_fieldTable->RemoveAll();
		m_listTable->RemoveAll(); 
		m_clearForm = false;
	}
	if (m_updateForm)
	{
		AddFormToPage();
		m_updateForm = false;
	}
}

void ComponentPageUI::Update(float dt)
{
	m_guiWindow->Update(dt);
}

void ComponentPageUI::Draw(sf::RenderWindow& rw)
{
	sfgui.Display(rw);
}

bool ComponentPageUI::LoadList(string filepath)
{
	m_componentList.LoadComponents(filepath);
	PopulateTable();
	return true;
}

void ComponentPageUI::PopulateTable()
{
	ComponentMap components;
	m_componentList.GetComponents(components);
	
	for (ComponentMap::iterator i = components.begin(); i != components.end(); ++i)
	{
		m_componentListCombo->AppendItem(i->second.name);
	}
	
	m_saveComponentBtn->Show(false);
	m_componentMainBox->Pack(m_saveComponentBtn);
}

void ComponentPageUI::GenerateForm(Component& comp)
{
	m_fieldEntries.clear();
	m_listEntries.clear();
	m_listFieldEntries.clear();

	m_componentNameEntry = sfg::Entry::Create(comp.name);
	if (comp.isItemComponent)
		m_isItemComponentCheckBtn->SetActive(true);
	else
		m_isItemComponentCheckBtn->SetActive(false);

	if (comp.fields.size() != 0)
	{

		for (int i = 0; i < comp.fields.size(); ++i)
		{
			auto fieldNameLabel = sfg::Label::Create("Name");
			auto fieldTypeLabel = sfg::Label::Create("Type");
			auto fieldNameEntry = sfg::Entry::Create(comp.fields[i].name);
			auto fieldTypeEntry = sfg::Entry::Create(comp.fields[i].valueType);
			m_fieldEntries.push_back(std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>(fieldNameEntry, fieldTypeEntry));
		}
	}
	
	if (comp.lists.size() == 0)
	{
		AddFormToPage();
		return;
	}

	for (int i = 0; i < comp.lists.size(); ++i)
	{
		List currList = comp.lists[i];
		auto listNameLabel = sfg::Label::Create("List Name");
		auto listEntryNameLabel = sfg::Label::Create("List Entry Name");
		auto listNameEntry = sfg::Entry::Create(currList.name);
		auto listEntryNameEntry = sfg::Entry::Create(currList.entryName);
		m_listEntries.push_back(std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>(listNameEntry, listEntryNameEntry));

		vector<std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>> listFields;
		for (int i = 0; i < currList.fields.size(); ++i)
		{
			auto fieldNameLabel = sfg::Label::Create("Name");
			auto fieldTypeLabel = sfg::Label::Create("Type");
			auto fieldNameEntry = sfg::Entry::Create(currList.fields[i].name);
			auto fieldTypeEntry = sfg::Entry::Create(currList.fields[i].valueType);
			listFields.push_back(std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>(fieldNameEntry, fieldTypeEntry));
		}
		m_listFieldEntries.push_back(listFields);
	}

	AddFormToPage();
}

void ComponentPageUI::AddFormToPage()
{
	int i = 0;
	int row = 0;

	sfg::Box::Ptr componentNameBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	sfg::Label::Ptr compName = sfg::Label::Create("Component Name");
	componentNameBox->Pack(compName);
	componentNameBox->Pack(m_componentNameEntry);
	componentNameBox->Pack(m_isItemComponentCheckBtn);

	m_componentEditBox->Pack(componentNameBox);
	auto fieldsBox = sfg::Box::Create();
	fieldsBox->Pack(sfg::Label::Create("Fields"));
	fieldsBox->Pack(m_addFieldBtn);
	m_componentEditBox->Pack(fieldsBox);

	for (i = 0; i < m_fieldEntries.size(); ++i)
	{
		auto fieldNameLabel = sfg::Label::Create("Name");
		auto fieldTypeLabel = sfg::Label::Create("Type");
		auto fieldNameEntry = m_fieldEntries[i].first;
		auto fieldTypeEntry = m_fieldEntries[i].second;

		m_fieldTable->Attach(fieldNameLabel, sf::Rect<sf::Uint32>(0, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
		m_fieldTable->Attach(fieldNameEntry, sf::Rect<sf::Uint32>(1, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
		m_fieldTable->Attach(fieldTypeLabel, sf::Rect<sf::Uint32>(2, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
		m_fieldTable->Attach(fieldTypeEntry, sf::Rect<sf::Uint32>(3, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
		row++;
	}

	m_componentEditBox->Pack(m_fieldTable);

	auto listBox = sfg::Box::Create();
	listBox->Pack(sfg::Label::Create("Lists"));

	if (m_listEntries.size() == 0)
	{
		listBox->Pack(m_addListBtn);
		m_componentEditBox->Pack(listBox);
		return;
	}
	
	listBox->Pack(m_addListFieldButton);
	m_componentEditBox->Pack(listBox);

	for (i = 0; i < m_listEntries.size(); ++i)
	{
		auto listNameLabel = sfg::Label::Create("List Name");
		auto listEntryNameLabel = sfg::Label::Create("List Entry Name");
		auto listNameEntry = m_listEntries[i].first;
		auto listEntryNameEntry = m_listEntries[i].second;
		m_listTable->Attach(listNameLabel, sf::Rect<sf::Uint32>(0, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
		m_listTable->Attach(listNameEntry, sf::Rect<sf::Uint32>(1, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
		m_listTable->Attach(listEntryNameLabel, sf::Rect<sf::Uint32>(2, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
		m_listTable->Attach(listEntryNameEntry, sf::Rect<sf::Uint32>(3, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
		row++;
		for (int j = 0; j < m_listFieldEntries[i].size(); ++j)
		{
			auto fieldNameLabel = sfg::Label::Create("Name");
			auto fieldTypeLabel = sfg::Label::Create("Type");
			auto fieldNameEntry = m_listFieldEntries[i][j].first;
			auto fieldTypeEntry = m_listFieldEntries[i][j].second;
			m_listTable->Attach(fieldNameLabel, sf::Rect<sf::Uint32>(0, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
			m_listTable->Attach(fieldNameEntry, sf::Rect<sf::Uint32>(1, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
			m_listTable->Attach(fieldTypeLabel, sf::Rect<sf::Uint32>(2, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
			m_listTable->Attach(fieldTypeEntry, sf::Rect<sf::Uint32>(3, row, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(5.f, 5.f));
			row++;
		}

		row++;
	}
	m_componentEditBox->Pack(m_listTable);

}

void ComponentPageUI::OnComponentComboSelect()
{
	string compName = m_componentListCombo->GetSelectedText();
	int id = reinterpret_cast<int>(HashedString::hash_name(compName.c_str()));
	Component selectedComp;
	if (!m_componentList.GetComponentByID(id, selectedComp))
		return;
	GenerateForm(selectedComp);
	m_saveComponentBtn->Show(true);
	m_clearForm = true;
	m_updateForm = true;
}

void ComponentPageUI::OnAddNewComponent()
{
	Component component;
	GenerateForm(component);
	m_saveComponentBtn->Show(true);
}

void ComponentPageUI::OnSaveComponentClick()
{
	Component component;

	component.name = m_componentNameEntry->GetText();
	component.isItemComponent = m_isItemComponentCheckBtn->IsActive();

	for (int i = 0; i < m_fieldEntries.size(); ++i)
	{
		Field newField;
		newField.name = m_fieldEntries[i].first->GetText(); 
		newField.valueType = m_fieldEntries[i].second->GetText();
		component.fields.push_back(newField);
	}

	for (int i = 0; i < m_listEntries.size(); ++i)
	{
		List newList;
		newList.name = m_listEntries[i].first->GetText();
		newList.entryName = m_listEntries[i].second->GetText();
		for (int j = 0; j < m_listFieldEntries[i].size(); ++j)
		{
			Field newField;
			newField.name = m_listFieldEntries[i][j].first->GetText();
			newField.valueType = m_listFieldEntries[i][j].second->GetText();
			newList.fields.push_back(newField);
		}
		component.lists.push_back(newList);
	}
	m_componentList.SetComponentByID(reinterpret_cast<int>(HashedString::hash_name(component.name.c_str())), component);
	while (m_componentListCombo->GetItemCount() > 0)
		m_componentListCombo->RemoveItem(0);
	PopulateTable();
	m_saveComponentBtn->Show(true);
}

void ComponentPageUI::OnAddFieldClick()
{
	auto fieldNameLabel = sfg::Label::Create("Name");
	auto fieldTypeLabel = sfg::Label::Create("Type");
	auto fieldNameEntry = sfg::Entry::Create("");
	auto fieldTypeEntry = sfg::Entry::Create("");
	m_fieldEntries.push_back(std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>(fieldNameEntry, fieldTypeEntry));
	m_clearForm = true;
	m_updateForm = true;

}
void ComponentPageUI::OnAddListClick()
{
	auto listNameLabel = sfg::Label::Create("List Name");
	auto listEntryNameLabel = sfg::Label::Create("List Entry Name");
	auto listNameEntry = sfg::Entry::Create("");
	auto listEntryNameEntry = sfg::Entry::Create("");
	m_listEntries.push_back(std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>(listNameEntry, listEntryNameEntry));

	vector<std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>> listFields;
	auto fieldNameLabel = sfg::Label::Create("Name");
	auto fieldTypeLabel = sfg::Label::Create("Type");
	auto fieldNameEntry = sfg::Entry::Create("");
	auto fieldTypeEntry = sfg::Entry::Create("");
	listFields.push_back(std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>(fieldNameEntry, fieldTypeEntry));

	m_listFieldEntries.push_back(listFields);
	
	m_clearForm = true;
	m_updateForm = true;

}
void ComponentPageUI::OnAddListFieldClick()
{
	auto fieldNameLabel = sfg::Label::Create("Name");
	auto fieldTypeLabel = sfg::Label::Create("Type");
	auto fieldNameEntry = sfg::Entry::Create("");
	auto fieldTypeEntry = sfg::Entry::Create("");

	m_listFieldEntries[0].push_back(std::pair<sfg::Entry::Ptr, sfg::Entry::Ptr>(fieldNameEntry, fieldTypeEntry));

	m_clearForm = true;
	m_updateForm = true;

}