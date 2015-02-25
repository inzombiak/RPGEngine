#ifndef COMPONENTFACTORY_H
#define COMPONENETFACTORY_H

#include <unordered_map>

#include "custom_defininitions.h"

using std::unordered_map;

class ComponentBase;
class ComponentFactory
{
private:
	typedef ComponentBase* (ComponentFactory::*ComponentCreationFunc)(void);

	template<class SubClass>
	ComponentBase* CreationFunction()
	{
		return new SubClass();
	};

public:
	template<class SubClass>
	bool Register(ComponentID id)
	{
		auto find = m_creationFunctions.find(id);
		if (find == m_creationFunctions.end())
		{
			m_creationFunctions[id] = &ComponentFactory::CreationFunction<SubClass>;
			return true;
		}
		return false;
	}

	ComponentBase* Create(ComponentID id)
	{
		auto find = m_creationFunctions.find(id);
		if (find != m_creationFunctions.end())
		{
			ComponentCreationFunc pFunc = find->second;
			return (this->*pFunc)();
		}

		return NULL;
	}

private:	
	unordered_map<ComponentID, ComponentCreationFunc> m_creationFunctions;
};

#endif
