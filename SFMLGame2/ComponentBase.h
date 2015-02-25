#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include "SFML\Graphics.hpp"
#include "custom_defininitions.h"
#include "tinyxml2.h"

using tinyxml2::XMLElement;

class ComponentBase
{
	friend class EntityFactory;

public:

	ComponentBase();
	virtual ~ComponentBase() {};

	virtual bool Init(const XMLElement* componentNode) = 0;
	virtual void PostInit() = 0;

	virtual void Update(float dt) {};
	
	void SetOwner(StrongEntityPtr owner);
	WeakEntityPtr GetOwner() const;
	virtual const char* GetName() = 0;
	virtual ComponentID GetID() { return GetIDFromName(GetName()); }
	static ComponentID GetIDFromName(const char* name) 
	{
		return reinterpret_cast<ComponentID>(HashedString::hash_name(name));
	}
	
protected:
	StrongEntityPtr m_owner;
};

#endif