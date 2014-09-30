#include "interfaces.h"

class EntityComponent
{
protected:
	StrongEntityPtr m_pOwner;

public:
	virtual ~EntityComponent(void) { m_pOwner.reset(); };

	virtual bool VInit() = 0;
	virtual void VPostInit(void) { }
	virtual void VUpdate(int deltaMs) { }
	virtual void VOnChanged(void) { }

	virtual ComponentID VGetID(void) const;

private:
	void SetOwner(StrongEntityPtr pOwner) // Set owner
	{
		m_pOwner = pOwner;
	}
};