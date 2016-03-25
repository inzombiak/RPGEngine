#include "..\Components\IScriptComponent.h"

class ScriptManager
{
public:
	void Update(float dt);

	static StrongComponentPtr CreateMovementScriptComponent();
private:
	static std::vector<std::shared_ptr<IScriptComponent>> m_scripts;
};

