#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "ComponentBase.h"

class InputComponent : public ComponentBase
{
public:
	virtual bool Init(const XMLElement* componentNode) override;
	virtual void PostInit() override {};

	const char* GetName()
	{
		return COMPONENT_NAME;
	}
	void Update(float dt);
	const static char* COMPONENT_NAME;

private:
	
};

#endif