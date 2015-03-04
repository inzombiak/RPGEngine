#include "TransformManager.h"

StrongComponentPtr TransformManager::CreateTransformComponent()
{
	shared_ptr<TransformComponent> newComponent(new TransformComponent);
	m_transformComponents.push_back(newComponent);
	return newComponent;
}

void TransformManager::Update(float dt)
{
	for (unsigned int i = 0; i < m_transformComponents.size(); ++i)
	{
		m_transformComponents[i]->Update(dt);
	}
}

vector<shared_ptr<TransformComponent>> TransformManager::m_transformComponents;