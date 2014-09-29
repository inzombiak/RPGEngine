#include "custom_defininitions.h"

class ObjectComponent
{
protected:
	StrongObjectPtr m_pOwner;

public:
	virtual ~ObjectComponent(void) { m_pOwner.reset(); };

	virtual bool VInit() = 0;
	virtual void VPostInit(void) { }
	virtual void VUpdate(int deltaMs) { }
	virtual void VOnChanged(void) { }

	virtual ComponentID VGetID(void) const;

private:
	void SetOwner(StrongObjectPtr pOwner) // Set owner
	{
		m_pOwner = pOwner;
	}
};