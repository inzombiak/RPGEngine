#include "InputManager.h"

StrongComponentPtr InputManager::CreateInputComponent()
{
	shared_ptr<InputComponent> newComponent(new InputComponent);
	m_inputComponents.push_back(newComponent);
	return newComponent;
}

void InputManager::Update(float dt, sf::Event event)
{
	for (unsigned int i = 0; i < m_inputComponents.size(); ++i)
	{
		if (!m_inputComponents[i]->GetInUse())
			m_inputComponents.erase(m_inputComponents.begin() + i);
		else
			m_inputComponents[i]->Update(dt, event);
	}
}

vector<shared_ptr<InputComponent>> InputManager::m_inputComponents;