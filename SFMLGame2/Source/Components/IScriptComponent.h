#pragma once

#include "IComponent.h"

class IScriptComponent : public IComponent
{
public:
	virtual void RegisterScriptFunctions() = 0;
	virtual void UnregisterScriptFunctions() = 0;
};

