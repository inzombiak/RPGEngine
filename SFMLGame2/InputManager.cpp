#include "InputManager.h"

StrongComponentPtr InputManager::CreateInputComponent()
{
	shared_ptr<InputComponent> newComponent(new InputComponent);
	m_inputComponents.push_back(newComponent);
	return newComponent;
}

void InputManager::Update(float dt)
{
	for (unsigned int i = 0; i < m_inputComponents.size(); ++i)
	{
		m_inputComponents[i]->Update(dt);
	}
}

vector<shared_ptr<InputComponent>> InputManager::m_inputComponents;