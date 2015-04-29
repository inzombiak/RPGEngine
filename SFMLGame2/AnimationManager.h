#pragma once
#include "custom_defininitions.h"

class AnimationComponent;

class AnimationManager
{
public:
	static StrongComponentPtr CreateAnimationComponent();

	void Update(float dt);

private:
	static std::vector<std::shared_ptr<AnimationComponent>> m_animationComponents;
};

