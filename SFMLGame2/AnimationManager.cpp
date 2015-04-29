#include "AnimationManager.h"
#include "AnimationComponent.h"

StrongComponentPtr AnimationManager::CreateAnimationComponent()
{
	std::shared_ptr<AnimationComponent> newAnimComp(new AnimationComponent());
	m_animationComponents.push_back(newAnimComp);
	return newAnimComp;
}

void AnimationManager::Update(float dt)
{
	for (unsigned int i = 0; i < m_animationComponents.size(); ++i)
		m_animationComponents[i]->Update(dt);
}

std::vector<std::shared_ptr<AnimationComponent>> AnimationManager::m_animationComponents;