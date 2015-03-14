#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "InputComponent.h"
#include <vector>

using std::vector;
using std::shared_ptr;
class InputManager
{
public:
	void Update(float dt, sf::Event event);
	static StrongComponentPtr CreateInputComponent();
private:
	static vector<shared_ptr<InputComponent>> m_inputComponents;
};

#endif