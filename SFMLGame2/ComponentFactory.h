#ifndef COMPONENTFACTORY_H
#define COMPONENETFACTORY_H

#include <unordered_map>

#include "custom_defininitions.h"

using std::unordered_map;

class ComponentBase;
class ComponentFactory
{
private:
	typedef StrongComponentPtr (*ComponentCreationFunc)(void);
public:
	template<class SubClass>
	bool Register(ComponentID id, StrongComponentPtr (*CreationFunction)(void))
	{
		auto find = m_creationFunctions.find(id);
		if (find == m_creationFunctions.end())
		{
			m_creationFunctions[id] = CreationFunction;
			return true;
		}
		return false;
	}

	StrongComponentPtr Create(ComponentID id)
	{
		auto find = m_creationFunctions.find(id);
		if (find != m_creationFunctions.end())
		{
			ComponentCreationFunc pFunc = find->second;
			StrongComponentPtr result = (*pFunc)();
			return result;
		}

		return NULL;
	}

private:	
	unordered_map<ComponentID, ComponentCreationFunc> m_creationFunctions;
};

#endif
