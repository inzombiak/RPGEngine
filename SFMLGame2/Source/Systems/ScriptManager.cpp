#include "ScriptManager.h"
#include "..\Components\MovementScriptComponent.h"

void ScriptManager::Update(float dt)
{
	for (int i = 0; i < m_scripts.size(); ++i)
		m_scripts[i]->Update(dt);
}

StrongComponentPtr ScriptManager::CreateMovementScriptComponent()
{
	std::shared_ptr<MovementScriptComponent> newMovementScript(new MovementScriptComponent());
	m_scripts.push_back(newMovementScript);
	return newMovementScript;
}

std::vector<std::shared_ptr<IScriptComponent>> ScriptManager::m_scripts;