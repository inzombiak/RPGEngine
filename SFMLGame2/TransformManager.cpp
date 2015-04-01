#include "TransformManager.h"

StrongComponentPtr TransformManager::CreateTransformComponent()
{
	shared_ptr<TransformComponent> newComponent(new TransformComponent);
	m_transformComponents.push_back(newComponent);
	return newComponent;
}

void TransformManager::Update(float dt)
{
	for (vector<shared_ptr<TransformComponent>>::iterator it = m_transformComponents.begin(); it != m_transformComponents.end();)
	{
		if (!(*it)->GetInUse())
			it = m_transformComponents.erase(it);
		else
		{
			(*it)->Update(dt);
			++it;
		}
	}
}

vector<shared_ptr<TransformComponent>> TransformManager::m_transformComponents;