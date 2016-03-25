#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "IComponent.h"

class InputComponent : public IComponent
{
public:
	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override {};

	const char* GetName()
	{
		return COMPONENT_NAME;
	}
	void Update(float dt, sf::Event);
	const static char* COMPONENT_NAME;

private:
	void SwitchAnimation(const string& message);
};

#endif