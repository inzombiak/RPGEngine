#pragma once
#include "ItemComponentBase.h"

#include <string>

class RestorationItemComponent : public ItemComponentBase
{
public:
	virtual bool Init(const XMLElement* componentNode) override;
	virtual void PostInit() override;
	void Update(float dt);

	const char* GetName()
	{
		return COMPONENT_NAME;
	}

	std::string GetEffect()
	{
		return m_effect;
	}

	int GetAmount()
	{
		return m_amount;
	}

	static const char* COMPONENT_NAME;
private:
	std::string m_effect;
	int m_amount;
};

