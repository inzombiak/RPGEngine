#include "UIMain.h"


UIMain::UIMain()
{

	m_guiWindow->SetTitle("Title");
	m_notebook->AppendPage(sfg::Label::Create(), sfg::Label::Create("Entities"));
	m_notebook->AppendPage(sfg::Label::Create(), sfg::Label::Create("Items"));
	m_notebook->AppendPage(m_componentMainBox, sfg::Label::Create("Components"));

	m_notebook->SetRequisition(sf::Vector2f(400.f, 0.f));
	m_notebook->SetScrollable(true);

	m_componentMainBox->Pack(m_componentSelectBox, true);
	m_componentMainBox->Pack(m_addNewComponent);
	m_componentMainBox->Pack(m_componentEditBox);

	m_guiWindow->Add(m_notebook);
}


UIMain::~UIMain()
{
}
