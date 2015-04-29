#pragma once

#include "ComponentBase.h"

class IScriptComponent : public ComponentBase
{
public:
	virtual void RegisterScriptFunctions() = 0;
	virtual void UnregisterScriptFunctions() = 0;
};

