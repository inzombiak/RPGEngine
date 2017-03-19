#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "..\Components\InputComponent.h"
#include <vector>

using std::vector;
using std::shared_ptr;
class UIManager;
class InputManager
{
public:
	void Update(float dt, sf::Event event, sf::RenderWindow& window);
	static StrongComponentPtr CreateInputComponent();
	void SetUI(UIManager* ui)
	{
		m_uiManager = ui;
	}
private:
	static vector<shared_ptr<InputComponent>> m_inputComponents;
	bool m_inventoryOpen = false;
	bool m_equipmentOpen = false;
	UIManager* m_uiManager;
};

#endif