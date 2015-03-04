#ifndef TRANSFORM_MANAGER_H
#define TRANSFORM_MANAGER_H

#include "TransformComponent.h"

#include <vector>

using std::vector;
using std::shared_ptr;

class TransformManager
{
public:
	static StrongComponentPtr CreateTransformComponent();
	void Update(float dt);

private:
	static vector<shared_ptr<TransformComponent>> m_transformComponents;
};

#endif